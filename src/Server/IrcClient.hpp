/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcClient.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hngo <hngo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 11:46:18 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/20 01:42:42 by hngo             ###   ########.fr       */
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
		void	sendResponse(const std::string& response) const;
		bool	getIsAuth();
		void	setAuth();
		void	setNickname(std::string nickname);
		void 	setUsername(std::string username);
		void 	setRealname(std::string realname);
		void 	setHostname(std::string hostname);
		void 	setServername(std::string servername);
        
        const std::string getNickname() const;
        std::string getUsername();
        std::string getRealname();
        std::string getHostname();
        std::string getServername();

        bool getUserAndNickSet() const;
};

#endif