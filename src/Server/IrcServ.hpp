/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 16:29:49 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/23 15:50:34 by jtsizik          ###   ########.fr       */
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
# include <signal.h>
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

        std::string _hostname;

	public:
		IrcServ(char *port, char *pass);
		~IrcServ();

		std::string	getPass();
        std::vector<IrcChannel>& getChannels();
        std::map<int, IrcClient>& getClients();
        IrcClient& getClientByNick(const std::string& nickname);
        std::string getHostname();
		void start();
		void setSocket();
		void handleDisconnect(int fd);
		void handleConnect();
		void receiveMessage(int fd);
		bool channelExists(const std::string& channelName) const;
		bool channelHasClient(const std::string& channelName, const IrcClient& client) const;
		IrcChannel *getChannelByName(const std::string& channelName);
		void kickClientFromChannel(const IrcClient& client, const std::string& channelName);

		bool nickInUse(const std::string &nick, IrcClient cl);

};

#endif