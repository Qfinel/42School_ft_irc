/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 14:36:42 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/16 14:03:48 by jtsizik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDHANDLER_HPP
# define COMMANDHANDLER_HPP

# include <iostream>
# include <string>
# include <sstream>

class IrcServ;
class IrcClient;
class Command;

class CommandHandler
{
	private:
		std::string		_buff;
		Command			*_comm;
		IrcServ			*_serv;
		IrcClient		*_client;
	public:
		CommandHandler(IrcServ *serv, IrcClient *cl, std::string buffer);
		~CommandHandler();
		Command* getCommand(const std::string& commandName);
	
		void handle();
};

#endif