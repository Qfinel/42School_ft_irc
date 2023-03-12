/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 16:29:49 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/12 12:05:04 by jtsizik          ###   ########.fr       */
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

class IrcServ
{
	private:
		const int			_port;
		const std::string	_pass;
		int					_socket;
		bool				_started;
	
		// std::vector<IrcClient>	_clients;
		// std::vector<IrcChannel>	_channels;

	public:
		IrcServ(char *port, char *pass);
		~IrcServ();

		void start();
		void setSocket();
		void receiveMsg();
		void handleDisconnect(int socket_fd);
		void handleConnect();
		void handleMessage(int socket_fd);

};

#endif