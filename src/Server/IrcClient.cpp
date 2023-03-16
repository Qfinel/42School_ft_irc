/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcClient.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 12:00:17 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/16 15:02:46 by jtsizik          ###   ########.fr       */
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

int		IrcClient::getFd()	{return (this->_fd);}

bool	IrcClient::getIsAuth()	{return (this->_isAuth);}

void	IrcClient::setAuth()	{this->_isAuth = true;}

void IrcClient::sendResponse(std::string response)
{
	std::string msg = response + "\r\n";
	if (send(this->_fd, msg.c_str(), msg.length(), 0) < 0)
		throw std::runtime_error("Failed sending response to the client");
}

void	IrcClient::setNickname(std::string nickname)	{this->_nickname = nickname;}

std::string	&IrcClient::getNickname()	{return (this->_nickname);}