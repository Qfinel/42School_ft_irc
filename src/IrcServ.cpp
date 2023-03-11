/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 16:31:54 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/11 19:10:57 by jtsizik          ###   ########.fr       */
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

void IrcServ::handleDisconnect(int socket_fd)
{
	return ;
}

void IrcServ::handleConnect()
{
	int					new_fd;
	struct sockaddr_in	address = {};

	//Accepting socket
	int new_fd = accept(this->_socket, (struct sockaddr*)&address, (socklen_t*)(sizeof(address)));
	if (new_fd < 0)
		throw std::runtime_error("Error while accepting socket");
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
	struct pollfd	fds[20];
	this->_started = true;
	int		fds_nb = 1;

	fds[0].fd = this->_socket;
	fds[0].events = POLLIN;
	fds[0].revents = 0;

	while (_started)
	{
		//Polling file descriptors (returns how many fds are ready)
		int ready = poll(fds, fds_nb, -1);
		if (ready < 0)
			throw std::runtime_error("Error while polling file descriptors");

		for (int i = 0; i < 20; i++)
		{
			if (fds[i].revents & POLLHUP) //End of the conneciton
			{
				handleDisconnect(fds[i].fd);
				return ;
			}
			else if ((fds[i].revents & POLLIN)) //Received some event
			{
				if (fds[i].fd == this->_socket)
				{
					handleConnect();
					return ;
				}
				handleMessage(fds[i].fd);
			}
		}
	}
}