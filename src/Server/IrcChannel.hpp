/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcChannel.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 15:30:42 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/14 15:31:49 by jtsizik          ###   ########.fr       */
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
};

#endif
