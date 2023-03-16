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
#include "Command.hpp"
#include "Server/IrcServ.hpp"
#include "Server/IrcClient.hpp"
#include "Command.hpp"

CommandHandler::CommandHandler(IrcServ *serv, IrcClient *cl, std::string buffer)
{
	this->_comm = buffer;
	this->_client = cl;
	this->_serv = serv;
}

CommandHandler::~CommandHandler()	{}

// void CommandHandler::handle()
// {
// 	std::istringstream	iss(this->_comm);
// 	std::string word1, word2;
// 	iss >> word1 >> word2;

// 	if (!this->_client->getIsAuth() && strcmp("PASS", word1.c_str()))
// 		this->_client->sendResponse("Please provide a server password using PASS command");
// 	else if (!this->_client->getIsAuth() && !strcmp("PASS", word1.c_str()))
// 	{
// 		if (!strcmp(word2.c_str(), this->_serv->getPass().c_str()))
// 		{
// 			this->_client->sendResponse("Congratualtions! You are authenticated!");
// 			this->_client->setAuth();
// 		}
// 		else
// 			this->_client->sendResponse("It's a wrong password ;(");
// 	}
// 	else
// 		std::cout << this->_comm;
// }


Command* CommandHandler::getCommand(const std::string& commandName) {
    if (commandName == "PASS") {
        return new PassCommand();
    } else if (commandName == "NICK") {
		return new NickCommand();
	}
	
    // Add other commands here, e.g. NICK, USER, JOIN, etc.
    return NULL;
}

void CommandHandler::handle() {
    std::istringstream iss(this->_comm);
    std::string commandName, word;
    std::vector<std::string> args;

    iss >> commandName;
    while (iss >> word) {
        args.push_back(word);
    }

    Command* command = getCommand(commandName);

    if (!this->_client->getIsAuth() && commandName != "PASS") {
        this->_client->sendResponse("Please provide a server password using PASS command");
    } else if (command != NULL) {
        command->execute(*_serv, *_client, args);
        delete command; // Don't forget to delete the command to prevent memory leaks
    } else {
        // Handle unrecognized commands or just print the received command
        std::cout << this->_comm;
    }
}
