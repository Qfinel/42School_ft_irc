/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 14:36:54 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/14 17:08:27 by jtsizik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"

CommandHandler::CommandHandler(IrcServ *serv, IrcClient *cl, std::string buffer)
{
	this->_comm = buffer;
	this->_client = cl;
	this->_serv = serv;
}

CommandHandler::~CommandHandler()	{}

void CommandHandler::handle()
{
	std::istringstream	iss(this->_comm);
	std::string word1, word2;
	iss >> word1 >> word2;

	if (!this->_client->getIsAuth() && strcmp("PASS", word1.c_str()))
		this->_client->sendResponse("Please provide a server password using PASS command");
	else if (!this->_client->getIsAuth() && !strcmp("PASS", word1.c_str()))
	{
		if (!strcmp(word2.c_str(), this->_serv->getPass().c_str()))
		{
			this->_client->sendResponse("Congratualtions! You are authenticated!");
			this->_client->setAuth();
		}
		else
			this->_client->sendResponse("It's a wrong password ;(");
	}
	else
		std::cout << this->_comm;
}