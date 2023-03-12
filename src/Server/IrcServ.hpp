/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 16:29:49 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/12 15:31:11 by jtsizik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
# define IRCSERV_HPP

# include <iostream>
# include <ctype.h>
# include <poll.h>
# include <fcntl.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <vector>
# include <unistd.h>
# include "IrcClient.hpp"

class IrcServ
{
	private:
		const int			_port;
		const std::string	_pass;
		int					_socket;
		bool				_started;
	
		std::vector<struct pollfd>	_fds;
		std::vector<IrcClient>		_clients;
		// std::vector<IrcChannel>	_channels;

	public:
		IrcServ(char *port, char *pass);
		~IrcServ();

		void start();
		void setSocket();
		void handleDisconnect(int index);
		void handleConnect();
		void receiveMessage(int fd);

};

#endif