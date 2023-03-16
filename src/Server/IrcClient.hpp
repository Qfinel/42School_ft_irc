/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcClient.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 11:46:18 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/16 15:07:19 by jtsizik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCCLIENT_HPP
# define IRCCLIENT_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>

class IrcClient
{
	private:
		int			_fd;
		bool		_isAuth;
		std::string	_nickname;
		std::string	_username;
		std::string	_realname;
		std::string	_hostname;
		std::string	_servername;

	public:
		IrcClient(int fd);
		~IrcClient();

		int		getFd();
		void	sendResponse(std::string response);
		bool	getIsAuth();
		void	setAuth();
		void	setNickname(std::string nickname);
		void 	setUsername(std::string username);
		void 	setRealname(std::string realname);
		void 	setHostname(std::string hostname);
		void 	setServername(std::string servername);
		std::string &getNickname();
};

#endif