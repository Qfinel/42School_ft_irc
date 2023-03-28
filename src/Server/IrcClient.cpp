/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcClient.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 12:00:17 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/28 13:51:00 by jtsizik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcClient.hpp"

IrcClient::IrcClient(int fd)
{
	this->_fd = fd;
	this->_isAuth = false;
	this->_nickname = "*";
	this->_username = "";
	this->_realname = "";
}

IrcClient::~IrcClient()	{}

int		IrcClient::getFd()	{return (this->_fd);}

bool	IrcClient::getIsAuth()	{return (this->_isAuth);}

void	IrcClient::setAuth()	{this->_isAuth = true;}

void IrcClient::sendResponse(const std::string& response) const
{
	std::string msg = response + "\r\n";
	if (send(this->_fd, msg.c_str(), msg.length(), 0) < 0)
		throw std::runtime_error("Failed sending response to the client");
}

void	IrcClient::setNickname(std::string nickname)	{this->_nickname = nickname;}

void 	IrcClient::setUsername(std::string username)	{this->_username = username;}

void 	IrcClient::setRealname(std::string realname)	{this->_realname = realname;}

void 	IrcClient::setHostname(std::string hostname)	{this->_hostname = hostname;}

void 	IrcClient::setServername(std::string servername)	{this->_servername = servername;}

const std::string IrcClient::getNickname() const { return this->_nickname; }

std::string IrcClient::getUsername() { return this->_username; }

std::string IrcClient::getRealname() { return this->_realname; }

std::string IrcClient::getHostname() { return this->_hostname; }

std::string IrcClient::getServername() { return this->_servername; }

bool IrcClient::getUserAndNickSet() const {
    return !_username.empty() && !_nickname.empty() && _nickname != "*";
}
