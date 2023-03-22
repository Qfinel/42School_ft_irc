/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcChannel.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 15:30:42 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/21 16:09:55 by jtsizik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	IRCCHANNEL_HPP
# define IRCCHANNEL_HPP

# include <iostream>
# include <vector>
# include <set>
# include "IrcClient.hpp"

class IrcChannel
{
	private:
		std::string		_name;
		std::vector<IrcClient*>	_clients;
        std::set<std::string> _modes;

	public:
		IrcChannel(const std::string& name);
		~IrcChannel();

		const std::string& getName() const;
		void addClient(IrcClient& client);
		void sendMessage(const std::string& message);
        const std::vector<IrcClient*>& getMembers() const;
		const IrcClient* getClientByName(const std::string& nickname) const;
		void kickClient(const IrcClient& client);
		size_t getNumClients() const;
        bool isMember(const IrcClient& client) const;
        void addMode(const std::string& mode);
        void removeMode(const std::string& mode);
        std::string getMode() const;
};

#endif
