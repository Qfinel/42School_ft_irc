/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hngo <hngo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 14:36:54 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/17 19:15:14 by hngo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include "Command.hpp"
#include "Server/IrcServ.hpp"
#include "Server/IrcClient.hpp"
#include "Command.hpp"

CommandHandler::CommandHandler(IrcServ *serv, IrcClient *cl, std::string buffer)
{
	this->_comm = NULL;
    this->_buff = buffer;
	this->_client = cl;
	this->_serv = serv;
}

CommandHandler::~CommandHandler()	{delete this->_comm;}

Command* CommandHandler::getCommand(const std::string& commandName) {
    if (commandName == "PASS") {
        return new PassCommand();
    } else if (commandName == "NICK") {
		return new NickCommand();
	} else if (commandName == "QUIT") {
		return new QuitCommand();
	} else if (commandName == "USER") {
		return new UserCommand();
	} else if (commandName == "JOIN") {
		return new JoinCommand();
	} else if (commandName == "PRIVMSG") {
        return new PrivmsgCommand();
    } else if (commandName == "NOTICE") {
        return new NoticeCommand();
    }
    // Add other commands here, e.g. NICK, USER, JOIN, etc.
    return NULL;
}

// void CommandHandler::handle() {
//     std::istringstream iss(this->_buff);
//     std::string commandName, word;
//     std::vector<std::string> args;

//     iss >> commandName;
//     while (iss >> word) {
//         args.push_back(word);
//     }

//     Command* command = getCommand(commandName);
//     this->_comm = command;

//     std::cout << this->_buff; // for debugging

//     if (!this->_client->getIsAuth() && commandName != "PASS" && commandName != "QUIT") {
//         this->_client->sendResponse("451 " + this->_client->getNickname() + " :Please provide a server password using PASS command");
//     } else if (command != NULL) {
//             if ((commandName != "USER" && commandName != "NICK" && commandName != "PASS" && commandName != "QUIT") && !this->_client->getUserAndNickSet()) {
//                 this->_client->sendResponse("461 " + this->_client->getNickname() + " :Please set both USER and NICK before using other commands");
//             } else {
//                 command->execute(*_serv, *_client, args);
//             }
//     }
//     // else {
//     //     // Handle unrecognized commands or just print the received command
//     //     std::cout << this->_buff;
//     // }
// }

// void CommandHandler::handle() {
//     std::istringstream iss(this->_buff);
//     std::string commandName, word;
//     std::vector<std::string> args;

//     iss >> commandName;
//     while (iss >> word) {
//         args.push_back(word);
//     }

//     Command* command = getCommand(commandName);
//     this->_comm = command;

//     std::cout << this->_buff; // for debugging

//     if (!this->_client->getIsAuth() && commandName != "PASS" && commandName != "QUIT") {
//         this->_client->sendResponse("451 " + this->_client->getNickname() + " :Please provide a server password using PASS command");
//     } else if (command != NULL) {
//             if ((commandName != "USER" && commandName != "NICK" && commandName != "PASS" && commandName != "QUIT") && !this->_client->getUserAndNickSet()) {
//                 this->_client->sendResponse("461 " + this->_client->getNickname() + " :Please set both USER and NICK before using other commands");
//             } else {
//                 command->execute(*_serv, *_client, args);
//             }
//     } else {
//         // // Handle unrecognized commands or just print the received command
//         // if (!this->_client->getIsAuth() && commandName != "PASS" && commandName != "QUIT") {
//         //     this->_client->sendResponse("451 " + this->_client->getNickname() + " :Please provide a server password using PASS command");
//         // }
//     }
// }

// void CommandHandler::handle() {
//     std::istringstream iss(this->_buff);
//     std::string commandName, word;
//     std::vector<std::string> args;

//     iss >> commandName;
//     while (iss >> word) {
//         args.push_back(word);
//     }

//     Command* command = getCommand(commandName);
//     this->_comm = command;
    
//     std::cout << "CommandHandler::handle() - commandName: " << commandName << std::endl;
//     std::cout << this->_buff; // for debugging

//     if (!this->_client->getIsAuth() && (commandName != "PASS" && commandName != "QUIT" && command != NULL)) {
//         this->_client->sendResponse("451 " + this->_client->getNickname() + " :Please provide a server password using PASS command");
//     } else if (command != NULL) {
//             if ((commandName != "USER" && commandName != "NICK" && commandName != "PASS" && commandName != "QUIT") && !this->_client->getUserAndNickSet()) {
//                 this->_client->sendResponse("461 " + this->_client->getNickname() + " :Please set both USER and NICK before using other commands");
//             } else {
//                 command->execute(*_serv, *_client, args);
//             }
//     } else {
//         // Handle unrecognized commands or just print the received command
//     }
// }

void CommandHandler::handle() {
    std::istringstream input(this->_buff);
    std::string line;
    std::cout << "CommandHandler::handle()" << std::endl;
    std::cout << this->_buff << std::endl; // for debugging

    while (std::getline(input, line)) {
        std::istringstream iss(line);
        std::string commandName, word;
        std::vector<std::string> args;

        iss >> commandName;
        while (iss >> word) {
            args.push_back(word);
        }

        Command* command = getCommand(commandName);
        this->_comm = command;


        if (!this->_client->getIsAuth() && (commandName != "PASS" && commandName != "QUIT" && command != NULL)) {
            this->_client->sendResponse("451 " + this->_client->getNickname() + " :Please provide a server password using PASS command");
        } else if (command != NULL) {
                if ((commandName != "USER" && commandName != "NICK" && commandName != "PASS" && commandName != "QUIT") && !this->_client->getUserAndNickSet()) {
                    this->_client->sendResponse("461 " + this->_client->getNickname() + " :Please set both USER and NICK before using other commands");
                } else {
                    command->execute(*_serv, *_client, args);
                }
        } else {
            // Handle unrecognized commands or just print the received command
        }
    }
}
