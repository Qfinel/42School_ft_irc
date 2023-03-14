/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcClient.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 12:00:17 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/14 15:03:58 by jtsizik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcClient.hpp"

IrcClient::IrcClient(int fd)
{
	this->_fd = fd;
	this->_isAuth = false;
	this->_nickname = "";
	this->_username = "";
	this->_realname = "";
}

IrcClient::~IrcClient()	{}

int	 IrcClient::getFd()	{return (this->_fd);}

void IrcClient::sendResponse(std::string response)
{
	std::string msg = response + "\n";
	send(this->_fd, msg.c_str(), msg.length(), 0);
}