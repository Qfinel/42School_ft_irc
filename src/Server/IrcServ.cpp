/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 16:31:54 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/12 13:08:44 by jtsizik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServ.hpp"

IrcServ::IrcServ(char *port, char *pass) : _port(atoi(port)), _pass(pass)
{
	setSocket();
	this->_started = false;
}

IrcServ::~IrcServ()	{}

void IrcServ::receiveMsg()
{
	//Receiving a message from the client
	return ;
}

void IrcServ::handleDisconnect(int index)
{
	std::cout << "Client disconnected!" << std::endl;

	int	fd_del = this->_fds[index].fd;

	close(fd_del);

	std::vector<struct pollfd>::iterator 	it = this->_fds.begin();
	for (int i = 0; i <= index; i++)
	{
		if (i == index)
			this->_fds.erase(it);
		it++;
	}

	std::vector<IrcClient>::iterator	it_cl = this->_clients.begin();
	while (it_cl != this->_clients.end())
	{
		if (it_cl->getFd() == fd_del)
		{
			this->_clients.erase(it_cl);
			break ;
		}
		it_cl++;
	}
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
	this->_clients.push_back(new_client);

	struct pollfd 	new_poll;
	new_poll.fd = client_fd;
	new_poll.events = POLLIN;
	new_poll.revents = 0;
	this->_fds.push_back(new_poll);
}

void IrcServ::handleMessage(int socket_fd)
{
	return ;
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
	this->_started = true;

	serv_poll.fd = this->_socket;
	serv_poll.events = POLLIN;
	serv_poll.revents = 0;
	this->_fds.push_back(serv_poll);

	std::cout << "Waiting for connections..." << std::endl;

	while (_started)
	{
		//Polling file descriptors (returns how many events are ready)
		int ready = poll(&this->_fds[0], this->_fds.size(), -1);
		if (ready < 0)
			throw std::runtime_error("Error while polling file descriptors");

		for (int i = 0; i < this->_fds.size(); i++)
		{
			if (this->_fds[i].revents & POLLHUP) //End of the conneciton
			{
				handleDisconnect(i);
				break ;
			}
			else if ((this->_fds[i].revents & POLLIN)) //Received some event
			{
				if (this->_fds[i].fd == this->_socket)
				{
					handleConnect();
					break ;
				}
				handleMessage(this->_fds[i].fd);
			}
		}
	}
}