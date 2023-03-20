/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcChannel.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hngo <hngo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 15:30:42 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/20 01:39:01 by hngo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	IRCCHANNEL_HPP
# define IRCCHANNEL_HPP

# include <iostream>
# include <vector>
# include "IrcClient.hpp"

class IrcChannel
{
	private:
		std::string		_name;
		std::vector<IrcClient*>	_clients;

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
};

#endif
