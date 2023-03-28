/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcChannel.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 15:31:52 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/28 18:56:59 by jtsizik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcChannel.hpp"

IrcChannel::IrcChannel(const std::string& name)
    : _name(name), _channel_is_invite_only(false)
{
    // Set default modes to +nt
    _modes.insert("n");
    _modes.insert("t");
    _topic = "";
}


IrcChannel::~IrcChannel() {}

const std::string& IrcChannel::getName() const {
	return _name;
}

void IrcChannel::addClient(IrcClient& client) {
    if (_clients.size() == 0)
        _operators.push_back(&client);
	_clients.push_back(&client);
}

void IrcChannel::addOperator(IrcClient& client) {
    _operators.push_back(&client);
}
void IrcChannel::addToInviteList(IrcClient *client) {
    _invited_clients.push_back(client);
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

const std::vector<IrcClient*>& IrcChannel::getOperators() const {
    return _operators;
}

const IrcClient* IrcChannel::getClientByName(const std::string& nickname) const {
    for (std::vector<IrcClient*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if ((*it)->getNickname() == nickname) {
            return *it;
        }
    }
    return (NULL);
}

std::string IrcChannel::getMode() const {
    std::string mode = "+";
    for (std::set<std::string>::const_iterator it = _modes.begin(); it != _modes.end(); ++it) {
        mode += *it;
    }
    return mode;
}

void IrcChannel::kickClient(const IrcClient& client) {

    for (std::vector<IrcClient*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (*it == &client) {
            _clients.erase(it);
            break ;
        }    
    }
    if (isOperator(client)) {
        for (std::vector<IrcClient*>::iterator it = _operators.begin(); it != _operators.end(); ++it) {
            if (*it == &client) {
                _operators.erase(it);
                return ;
            } 
        }
    }
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

bool IrcChannel::isOperator(const IrcClient& client) const {
    for (std::vector<IrcClient*>::const_iterator it = _operators.begin(); it != _operators.end(); ++it) {
        const IrcClient* oper = *it;
        if (&client == oper) {
            return true;
        }
    }
    return false;
}

void IrcChannel::addMode(const std::string& mode) {
    _modes.insert(mode);
}

void IrcChannel::removeMode(const std::string& mode) {
    _modes.erase(mode);
}

bool IrcChannel::hasMode(const std::string& mode) const {
    for (std::set<std::string>::const_iterator it = _modes.begin(); it != _modes.end(); ++it) {
        if (*it == mode)
            return (true);
    }
    return (false);
}

bool IrcChannel::isInvited(const IrcClient& client) const {
    for (std::vector<IrcClient*>::const_iterator it = _invited_clients.begin(); it != _invited_clients.end(); ++it) {
        const IrcClient* member = *it;
        if (&client == member) {
            return true;
        }
    }
    return false;
}

const std::string& IrcChannel::getTopic() const {return (_topic);}

void IrcChannel::setTopic(const std::string &topic) {_topic = topic;}

bool IrcChannel::isInviteOnly() const{
    return _channel_is_invite_only;
}

std::vector<IrcChannel*> IrcChannel::_invite_only_channels;

void IrcChannel::setInviteOnly(bool invite_only) {
    _channel_is_invite_only = invite_only;

    if (_channel_is_invite_only) {
        // Add the channel to the list of invite-only channels
        _invite_only_channels.push_back(this);
    } else {
        // Remove the channel from the list of invite-only channels
        std::vector<IrcChannel*>::iterator it = std::find(_invite_only_channels.begin(), _invite_only_channels.end(), this);
        if (it != _invite_only_channels.end()) {
            _invite_only_channels.erase(it);
        }
    }
}

const std::vector<IrcChannel*>& IrcChannel::getInviteOnlyChannels() {
    return _invite_only_channels;
}


void IrcChannel::removeOperator(IrcClient& client)
{
    for (std::vector<IrcClient*>::iterator it = _operators.begin(); it != _operators.end(); ++it) {
        if (&client == *it) {
            _operators.erase(it);
            break ;
        }    
    }
}