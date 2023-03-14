/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 14:36:54 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/14 15:14:09 by jtsizik          ###   ########.fr       */
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
	
}