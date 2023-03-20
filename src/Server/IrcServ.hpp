/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hngo <hngo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 16:29:49 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/20 01:40:55 by hngo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
# define IRCSERV_HPP

# include <ctype.h>
# include <poll.h>
# include <fcntl.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <vector>
# include <map>
# include <unistd.h>
# include "IrcClient.hpp"
# include "IrcChannel.hpp"

class IrcServ
{
	private:
		const int			_port;
		const std::string	_pass;
		int					_socket;
		bool				_running;
	
		std::vector<struct pollfd>	_fds;
		std::map<int, IrcClient>	_clients;
		std::vector<IrcChannel>	_channels;

	public:
		IrcServ(char *port, char *pass);
		~IrcServ();

		std::string	getPass();
		void start();
		void setSocket();
		void handleDisconnect(int fd);
		void handleConnect();
		void receiveMessage(int fd);
		void joinChannel(IrcClient &client, const std::string &channelName);
        void sendChannelMessage(IrcClient& sender, const std::string& channelName, const std::string& message);
        void sendPrivateMessage(IrcClient& sender, const std::string& targetUser, const std::string& message);
        void sendMessage(IrcClient& sender, const std::string& target, const std::string& message);
		bool channelExists(const std::string& channelName) const;
		bool channelHasClient(const std::string& channelName, const IrcClient& client) const;
		IrcChannel& getChannelByName(const std::string& channelName);
		void kickClientFromChannel(const IrcClient& client, const std::string& channelName);
};

#endif