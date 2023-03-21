/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcChannel.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 15:31:52 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/21 16:09:53 by jtsizik          ###   ########.fr       */
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

const IrcClient* IrcChannel::getClientByName(const std::string& nickname) const {
    for (std::vector<IrcClient*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if ((*it)->getNickname() == nickname) {
            return *it;
        }
    }
    throw std::runtime_error("No such client: " + nickname);
}

void IrcChannel::kickClient(const IrcClient& client) {
    for (std::vector<IrcClient*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (*it == &client) {
            _clients.erase(it);
            client.sendResponse("You have been kicked from the channel: " + _name);
            return;
        }
    }
    throw std::runtime_error("No such client in the channel: " + client.getNickname());
}

bool IrcChannel::isMember(const IrcClient& client) const {
    for (std::vector<IrcClient*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        const IrcClient* member = *it;
        if (&client == member) {
            return true;
        }
    }
    return false;
}
