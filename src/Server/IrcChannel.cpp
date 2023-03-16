/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcChannel.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 15:31:52 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/14 15:32:24 by jtsizik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcChannel.hpp"

IrcChannel::IrcChannel(const std::string& name) : _name(name) {}

IrcChannel::~IrcChannel() {}

const std::string& IrcChannel::getName() const {
	return _name;
}

void IrcChannel::addClient(IrcClient& client) {
	_clients.push_back(&client);
}

void IrcChannel::sendMessage(const std::string& message) {
    for (std::vector<IrcClient*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        IrcClient* client = *it;
        client->sendResponse(message);
    }
}

const std::vector<IrcClient*>& IrcChannel::getMembers() const {
    return _clients;
}