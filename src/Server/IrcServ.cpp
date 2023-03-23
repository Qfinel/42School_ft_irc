/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hngo <hngo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 16:31:54 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/23 18:46:53 by hngo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServ.hpp"
# include "../CommandHandler.hpp"

IrcServ::IrcServ(char *port, char *pass) : _port(atoi(port)), _pass(pass)
{
    // Allocate a buffer to store the hostname
    const int HOSTNAME_BUFFER_SIZE = 256;
    char hostnameBuffer[HOSTNAME_BUFFER_SIZE];

    // Get the hostname
    if (gethostname(hostnameBuffer, HOSTNAME_BUFFER_SIZE) == 0) {
        _hostname = std::string(hostnameBuffer);
    } else {
        // If getting the hostname fails, use "localhost" or "127.0.0.1" as a fallback
        _hostname = "localhost";
    }
	setSocket();
	this->_running = false;
}

IrcServ::~IrcServ()	{}

std::string	IrcServ::getPass()	{return (this->_pass);}

std::vector<IrcChannel>& IrcServ::getChannels()	{return (this->_channels);}

std::map<int, IrcClient>& IrcServ::getClients()	{return (this->_clients);}

IrcClient& IrcServ::getClientByNick(const std::string& nickname)
{
    for (std::map<int, IrcClient>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (it->second.getNickname() == nickname)
            return it->second;
    }
    throw std::runtime_error("Client not found");
}

std::string IrcServ::getHostname() { return _hostname; }

void IrcServ::handleDisconnect(int fd)
{
	std::cout << "Client disconnected!" << std::endl;

	IrcClient client = _clients.at(fd);

	for (std::vector<IrcChannel>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		if (it->isMember(client))
			it->kickClient(client);
	}

	close(fd);

	for (std::vector<struct pollfd>::iterator it = this->_fds.begin(); it != this->_fds.end(); it++)
	{
		if (it->fd == fd)
		{
			this->_fds.erase(it);
			break ;
		}
	}

	this->_clients.erase(fd);
	if (_clients.size() == 0)
		_running = false;
}

void IrcServ::handleConnect()
{
	struct sockaddr_in	address = {};
	socklen_t add_size = sizeof(address);

	//Accepting socket
	int client_fd = accept(this->_socket, (struct sockaddr*)&address, &add_size);
	if (client_fd < 0)
		throw std::runtime_error("Error while accepting socket");
	
	std::cout << "New client connected!" << std::endl;

	IrcClient new_client(client_fd);
	this->_clients.insert(std::make_pair(client_fd, new_client));

	// try
	// {
	// 	new_client.sendResponse("Welcome on board! Please provide a server password using PASS command");
	// }
	// catch (const std::runtime_error& e)
	// {
	// 	std::cerr << e.what() << '\n';
	// }

	struct pollfd 	new_poll;
	new_poll.fd = client_fd;
	new_poll.events = POLLIN;
	new_poll.revents = 0;
	this->_fds.push_back(new_poll);
}

void IrcServ::receiveMessage(int fd)
{	
	//Receiving a message from the client
	char	buf[1024];

	bzero(buf, 1024);
	if (recv(fd, buf, 1024, 0) < 0)
		throw std::runtime_error("Error while receiving a message");

	CommandHandler	command(this, &this->_clients.at(fd), buf);

	try
	{
		command.handle();
	}
	catch (std::runtime_error &e)
	{
		std::cout << e.what() << std::endl;
	}
}

void IrcServ::setSocket()
{
	//Creating IPV4/TCP socket
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
		throw std::runtime_error("Error while creating socket");
	
	//Setting opt (Allows a socket to bind to a port that's still in use by another socket)
	//Helps avoiding "address already in use" errors when restarting a server
	int opt = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    	throw std::runtime_error("Error while setting opt");

	//Setting to non-blocking mode
	if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("fnctl function failed");

	//Binding socket with IP and PORT
	struct sockaddr_in	address;

	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //All available network interfaces
    address.sin_port = htons(this->_port); //Convert to standartisized network byte order

	if (bind(socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
		throw std::runtime_error("Error while binding socket");
	
	//Listening for connections
	if (listen(socket_fd, 20) < 0)
		throw std::runtime_error("Error while listening for incoming connections");

	this->_socket = socket_fd;
}

void IrcServ::start()
{
	struct pollfd serv_poll;
	this->_running = true;

	serv_poll.fd = this->_socket;
	serv_poll.events = POLLIN;
	serv_poll.revents = 0;
	this->_fds.push_back(serv_poll);

	std::cout << "Waiting for connections..." << std::endl;

	while (_running)
	{
		//Polling file descriptors (returns how many events are ready)
		int ready = poll(&this->_fds[0], this->_fds.size(), -1);
		if (ready < 0)
			throw std::runtime_error("Error while polling file descriptors");

		for (int i = 0; i < (int)this->_fds.size(); i++)
		{
			if (this->_fds[i].revents == 0) //Skip iteration if no events
				continue ;
			if (this->_fds[i].revents & POLLHUP) //End of the conneciton
			{
				handleDisconnect(this->_fds[i].fd);
				break ;
			}
			else if ((this->_fds[i].revents & POLLIN)) //Received some event
			{
				if (this->_fds[i].fd == this->_socket)
				{
					handleConnect();
					break ;
				}
				receiveMessage(this->_fds[i].fd);
			}
		}
	}
	close(_socket);
}

class ChannelNameMatcher {
public:
    ChannelNameMatcher(const std::string& channelName) : _channelName(channelName) {}

    bool operator()(const IrcChannel& channel) const {
        return channel.getName() == _channelName;
    }

private:
    std::string _channelName;
};

bool IrcServ::channelExists(const std::string& channelName) const {
    for (std::vector<IrcChannel>::const_iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if (it->getName() == channelName) {
            return true;
        }
    }
    return false;
}

bool IrcServ::channelHasClient(const std::string& channelName, const IrcClient& client) const {
    for (std::vector<IrcChannel>::const_iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if (it->getName() == channelName) {
            return it->getClientByName(client.getNickname()) != NULL;
        }
    }
    return false;
}

IrcChannel *IrcServ::getChannelByName(const std::string& channelName) {
    std::vector<IrcChannel>::iterator it = std::find_if(_channels.begin(), _channels.end(), ChannelNameMatcher(channelName));
    if (it != _channels.end()) {
        return it.base();
    } else {
        throw std::runtime_error("No such channel: " + channelName);
    }
}

size_t IrcChannel::getNumClients() const {
    return _clients.size();
}

void IrcServ::kickClientFromChannel(const IrcClient& client, const std::string& channelName) {
    std::vector<IrcChannel>::iterator it = std::find_if(_channels.begin(), _channels.end(), ChannelNameMatcher(channelName));
    if (it != _channels.end()) {
        it->kickClient(client);
        if (it->getNumClients() == 0) {
            _channels.erase(it);
        }
    } else {
        throw std::runtime_error("No such channel: " + channelName);
    }
}

bool IrcServ::nickInUse(const std::string &nick, IrcClient cl)
{
	for (std::map<int, IrcClient>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		if (nick == it->second.getNickname() && it->second.getFd() != cl.getFd())
			return (true);
	}
	return (false);
}

void IrcServ::inviteUserToChannel(const std::string& nickname, const std::string& channelName) {
    
	// Check if channel exits
    IrcChannel* channel = getChannelByName(channelName);
    if (!channel) {
        throw std::runtime_error("403 " + channelName + " :No such channel\r\n");
        return;
    }

	//check if sender is oper
	
	const IrcClient* user = channel->getClientByName(nickname);
	if (!user) {
    throw std::runtime_error("401 " + nickname + " :No such nick\r\n");
    return;
}

	
	// Check if user is already a member of the channel
    if (channel->isMember(*user)) {
        throw std::runtime_error("443 " + nickname + " " + channelName + " :is already on channel\r\n");
        return;
    }

	//check if invite only

	//put invitee on invite list
	channel->addToInviteList(*user); // need to check in JOIN command if user is on _invited_clients;
	channel->sendMessage("341 " + nickname + " " + channelName);
	
	//
}