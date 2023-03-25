/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcChannel.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hngo <hngo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 15:30:42 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/25 13:51:25 by hngo             ###   ########.fr       */
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
		std::vector<IrcClient*>	_operators;
        std::set<std::string> _modes;
		std::string		_topic;
		std::vector<IrcClient*> _invited_clients;
		static std::vector<IrcChannel*> _invite_only_channels;
		bool _channel_is_invite_only;

	public:
		IrcChannel(const std::string& name);
		~IrcChannel();

		const std::string& getName() const;
		void addClient(IrcClient& client);
        void addOperator(IrcClient& client);
		void addToInviteList(IrcClient *client);
		void sendMessage(const std::string& message);
        const std::vector<IrcClient*>& getMembers() const;
		const std::vector<IrcClient*>& getOperators() const;
		const IrcClient* getClientByName(const std::string& nickname) const;
		void kickClient(const IrcClient& client);
		size_t getNumClients() const;
        bool isMember(const IrcClient& client) const;
		bool isOperator(const IrcClient& client) const;
		bool isInvited(const IrcClient& client) const;
        void addMode(const std::string& mode);
        void removeMode(const std::string& mode);
        std::string getMode() const;
		const std::string& getTopic() const;
		void setTopic(const std::string &topic);
		bool hasMode(const std::string& mode) const;
		bool isInviteOnly() const;
		void setInviteOnly(bool invite_only);
		static const std::vector<IrcChannel*>& getInviteOnlyChannels();
};

#endif
