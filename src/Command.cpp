#include "Command.hpp"
#include "Server/IrcServ.hpp"
#include "Server/IrcClient.hpp"
#include <sstream>
#include <ctime>

void PassCommand::execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) {
    if (args.size() < 1) {
        client.sendResponse("461 " + client.getNickname() + " PASS"); // 461: ERR_NEEDMOREPARAMS
        return;
    }
    // Find the colon (:) to parse the password.
    size_t colonPos = args[0].find(':');
    std::string password;

    if (colonPos != 0) {
        password = args[0];
    } else {
        password = args[0].substr(colonPos + 1);
        for (size_t i = 1; i < args.size(); ++i) {
            password += " " + args[i];
        }
    }
    
    if (!client.getIsAuth()) {
        if (password == server.getPass()) {
            client.setAuth();
        } else {
            client.sendResponse("464 " + client.getNickname() + " :Password incorrect "); // 464: ERR_PASSWDMISMATCH
        }
    } else {
        client.sendResponse("462 " + client.getNickname() + " :You're already registered"); // 462: ERR_ALREADYREGISTRED
    }
}


void NickCommand::execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) {
	if (args.size() != 1) {
        client.sendResponse("461 " + client.getNickname() + " NICK");
        return;
    }

    // Set the client's nickname and send a response.
    if (!server.nickInUse(args[0], client))
        client.setNickname(args[0]);
    else
    {
        client.sendResponse("433 " + client.getNickname() + " " + args[0] + " :Nickname is already in use");
        return ;
    }
	// client.sendResponse("Your nickname is now " + args[0]);
    if (client.getUserAndNickSet()) {
        client.sendResponse("001 " + client.getNickname() + " :Welcome to the Internet Relay Network " + client.getNickname() + "!"); // 001: RPL_WELCOME
        client.sendResponse("002 " + client.getNickname() + " :Your host is " + client.getServername() + ", running version 1.0"); // 002: RPL_YOURHOST
    }
}

void QuitCommand::execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) {
    (void)args;
    server.handleDisconnect(client.getFd());
}

void UserCommand::execute(IrcServ&, IrcClient& client, const std::vector<std::string>& args) {
    if (args.size() < 4) {
        client.sendResponse("461 " + client.getNickname() + " USER"); //  "Usage: USER <username> <hostname> <servername> <realname>"
        return;
    }

    std::string username = args[0];
    std::string hostname = args[1];
    std::string servername = args[2];

    // Find the colon (:) to parse the real name.
    size_t colonPos = args[3].find(':');
    std::string realname;

    if (colonPos != 0) {
        realname = args[1];
    } else {
        realname = args[1].substr(colonPos + 1);
        for (size_t i = 2; i < args.size(); ++i) {
            realname += " " + args[i];
        }
    }

    // Set the client's username, real name, and other information.
    client.setUsername(username);
    client.setHostname(hostname);
    client.setServername(servername);
    client.setRealname(realname);

    // Sending a welcome message
    // client.sendResponse("Welcome to the Internet Relay Network " + username + "!");
    if (client.getUserAndNickSet()) {
        client.sendResponse("001 " + client.getNickname() + " :Welcome to the Internet Relay Network, " + client.getNickname()); // 001: RPL_WELCOME
        client.sendResponse("002 " + client.getNickname() + " :Your host is " + servername + ", running version 1.0"); // 002: RPL_YOURHOST
    }

    // Sending your host message
    // client.sendResponse("Your host is " + servername + ", running version 1.0");
}

void JoinCommand::joinChannel(IrcClient &client, const std::string &channelName) {
    // Check if the channel exists; if not, create it
    std::vector<IrcChannel>::iterator it = std::find_if(_server.getChannels().begin(), _server.getChannels().end(), ChannelNameMatcher(channelName));

    // If the channel doesn't exist, create a new one and add it to the _channels vector
    if (it == _server.getChannels().end()) {
        IrcChannel newChannel(channelName);
        newChannel.addClient(client);
        _server.getChannels().push_back(newChannel);
        client.sendResponse(":" + client.getNickname() + " JOIN :" + channelName);
    } else if (!it->isMember(client)){
        // Add the client to the existing channel
        it->addClient(client);
        client.sendResponse(":" + client.getNickname() + " JOIN :" + channelName);
    }
    // Notify the client that they have joined the channel
    // client.sendResponse(":" + client.getNickname() + " JOIN :" + channelName);
}

void JoinCommand::execute(IrcServ&, IrcClient& client, const std::vector<std::string>& args) {
    if (args.size() != 1) {
        // Send an error message to the client.
        client.sendResponse("461 " + client.getNickname() + " JOIN");
        return;
    }
    // Make the client join the specified channel.
    joinChannel(client, args[0]);
}

void PrivmsgCommand::sendPrivateMessage(IrcClient& sender, const std::string& targetUser, const std::string& message) {
    for (std::map<int, IrcClient>::iterator it = _server.getClients().begin(); it != _server.getClients().end(); ++it) {
        if (it->second.getNickname() == targetUser) {
            std::string response = ":" + sender.getNickname() + " PRIVMSG " + targetUser + " :" + message + "\r\n";
            it->second.sendResponse(response);
            return;
        }
    }
    // Target user not found; you can handle this case if needed.
}

// void PrivmsgCommand::sendChannelMessage(IrcClient& sender, const std::string& channelName, const std::string& message) {
//     for (std::vector<IrcChannel>::iterator channel_it = _server.getChannels().begin(); channel_it != _server.getChannels().end(); ++channel_it) {
//         if (channel_it->getName() == channelName) {
//             std::string response = ":" + sender.getNickname() + " PRIVMSG " + channelName + " :" + message + "\r\n";
//             const std::vector<IrcClient*>& members = channel_it->getMembers();
//             for (std::vector<IrcClient*>::const_iterator client_it = members.begin(); client_it != members.end(); ++client_it) {
//                 if ((*client_it) != &sender) { // Only send the message to clients other than the sender
//                     (*client_it)->sendResponse(response);
//                 }
//             }
//             return;
//         }
//     }
//     // Channel not found; handle this case if needed.
// }

void PrivmsgCommand::sendChannelMessage(IrcClient& sender, const std::string& channelName, const std::string& message) {
    for (std::vector<IrcChannel>::iterator channel_it = _server.getChannels().begin(); channel_it != _server.getChannels().end(); ++channel_it) {
        if (channel_it->getName() == channelName) {
            if (!channel_it->isMember(sender)) {
                sender.sendResponse(":" + _server.getHostname() + " 404 " + sender.getNickname() + " " + channelName + " :You cannot send external messages to this channel whilst the +n (noextmsg) mode is set.\r\n");
                return;
            }

            std::string response = ":" + sender.getNickname() + " PRIVMSG " + channelName + " :" + message + "\r\n";
            const std::vector<IrcClient*>& members = channel_it->getMembers();
            for (std::vector<IrcClient*>::const_iterator client_it = members.begin(); client_it != members.end(); ++client_it) {
                if ((*client_it) != &sender) { // Only send the message to clients other than the sender
                    (*client_it)->sendResponse(response);
                }
            }
            return;
        }
    }
    // Channel not found; handle this case if needed.
}

void PrivmsgCommand::sendMessage(IrcClient& sender, const std::string& target, const std::string& message) {
    // If the target starts with '#' or '&', it's a channel; otherwise, it's a user.
    if (target[0] == '#' || target[0] == '&') {
        sendChannelMessage(sender, target, message);
    } else {
        sendPrivateMessage(sender, target, message);
    }
}

void PrivmsgCommand::execute(IrcServ&, IrcClient& client, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        // Send an error message to the client.
        client.sendResponse("461 " + client.getNickname() + " PRIVMSG");
        return;
    }

    // Find the colon (:) to parse the message.
    size_t colonPos = args[1].find(':');
    std::string message;

    if (colonPos != 0) {
        message = args[1];
    } else {
        message = args[1].substr(colonPos + 1);
        for (size_t i = 2; i < args.size(); ++i) {
            message += " " + args[i];
        }
    }

    // Send the message from the client to the specified target.
    sendMessage(client, args[0], message);
}

void NoticeCommand::sendPrivateMessage(IrcClient& sender, const std::string& targetUser, const std::string& message) {
    for (std::map<int, IrcClient>::iterator it = _server.getClients().begin(); it != _server.getClients().end(); ++it) {
        if (it->second.getNickname() == targetUser) {
            std::string response = ":" + sender.getNickname() + " NOTICE " + targetUser + " :" + message + "\r\n";
            it->second.sendResponse(response);
            return;
        }
    }
    // Target user not found; you can handle this case if needed.
}

void NoticeCommand::sendChannelMessage(IrcClient& sender, const std::string& channelName, const std::string& message) {
    for (std::vector<IrcChannel>::iterator channel_it = _server.getChannels().begin(); channel_it != _server.getChannels().end(); ++channel_it) {
        if (channel_it->getName() == channelName) {
            std::string response = ":" + sender.getNickname() + " NOTICE " + channelName + " :" + message + "\r\n";
            const std::vector<IrcClient*>& members = channel_it->getMembers();
            for (std::vector<IrcClient*>::const_iterator client_it = members.begin(); client_it != members.end(); ++client_it) {
                if ((*client_it) != &sender) { // Only send the message to clients other than the sender
                    (*client_it)->sendResponse(response);
                }
            }
            return;
        }
    }
    // Channel not found; handle this case if needed.
}

void NoticeCommand::sendMessage(IrcClient& sender, const std::string& target, const std::string& message) {
    // If the target starts with '#' or '&', it's a channel; otherwise, it's a user.
    if (target[0] == '#' || target[0] == '&') {
        sendChannelMessage(sender, target, message);
    } else {
        sendPrivateMessage(sender, target, message);
    }
}



void NoticeCommand::execute(IrcServ&, IrcClient& client, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        client.sendResponse("461 " + client.getNickname() + " NOTICE");
        return ;
    }
    // Find the colon (:) to parse the message.
    size_t colonPos = args[1].find(':');
    std::string message;

    if (colonPos != 0) {
        message = args[1];
    } else {
        message = args[1].substr(colonPos + 1);
        for (size_t i = 2; i < args.size(); ++i) {
            message += " " + args[i];
        }
    }

    sendMessage(client, args[0], message);
}

void PingCommand::execute(IrcServ&, IrcClient& client, const std::vector<std::string>& args) {
    if (args.size() < 1) {
        // Send an error message to the client.
        client.sendResponse("461 " + client.getNickname() + " PING");
        return;
    }
    // Send a PONG response to the client.
     client.sendResponse("PONG " + client.getNickname() + " 127.0.0.1");
}

void KickCommand::execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        // Send an error message to the client.
        client.sendResponse("461 " + client.getNickname() + " KICK"); // 461: ERR_NEEDMOREPARAMS
        return;
    }

    std::string channel = args[0];
    std::string nickname = args[1];

    if (server.channelExists(channel)) {
        if (server.channelHasClient(channel, client)) {
            const IrcChannel *ircChannel = server.getChannelByName(channel);
            const IrcClient* nicknameClient = ircChannel->getClientByName(nickname);
            if (nicknameClient != &client && ircChannel->isOperator(client)) { // check if the client to be kicked is not the one issuing the command
                server.kickClientFromChannel(*nicknameClient, channel);
                if (args.size() > 2) {
                    std::string reason = args[2];
                    for (size_t i = 3; i < args.size(); i++)
                        reason += " " + args[i];
                    nicknameClient->sendResponse(":" + client.getNickname() + " KICK " + channel + " " + nickname + " :" + reason);
                }
                else
                    nicknameClient->sendResponse(":" + client.getNickname() + " KICK " + channel + " " + nickname + " :No reason");
            } else if (!ircChannel->isOperator(client)){
                client.sendResponse("482 " + client.getNickname() + " " + channel + " :You're not channel operator"); // 482: ERR_CHANOPRIVSNEEDED
            } else {
                client.sendResponse(client.getNickname() + " " + channel + " :You can't kick yourself");
            }
        } else {
            client.sendResponse("442 " + client.getNickname() + " " + channel + " :You're not on that channel"); // 442: ERR_NOTONCHANNEL
        }
    } else {
        client.sendResponse("403 " + client.getNickname() + " " + channel + " :No such channel"); // 403: ERR_NOSUCHCHANNEL
    }
}

void CapCommand::execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) {
    (void)server;
    if (args.size() > 0 && args[0] == "LS")
        client.sendResponse("CAP * LS");
}

void WhoCommand::execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) {
    if (args.size() > 0 && !server.channelExists(args[0]))
    {
        client.sendResponse("403 " + client.getNickname() + " " + args[0] + " :No such channel"); // 403: ERR_NOSUCHCHANNEL
        return ;
    }

    if (args.size() == 0) {
        for (std::map<int, IrcClient>::iterator it = server.getClients().begin(); it != server.getClients().end(); it++)
            client.sendResponse("352 " + client.getNickname() + " * " + it->second.getUsername() + \
            " * ft_irc " + it->second.getNickname() + " H :0 " + it->second.getRealname());
    } else if (args.size() == 1) {
        IrcChannel *channel = server.getChannelByName(args[0]);
        std::vector<IrcClient*> vect = channel->getMembers();
        std::string channel_name = channel->getName();
        channel_name.erase(0, 1);
        for (std::vector<IrcClient*>::iterator it = vect.begin(); it != vect.end(); it++) {
            client.sendResponse("352 " + client.getNickname() + " " + channel_name + " " + (*it)->getUsername() + \
            " * ft_irc " + (*it)->getNickname() + " H :0 " + (*it)->getRealname());
        }
    } else if (args[1] == "o") {
        IrcChannel *channel = server.getChannelByName(args[0]);
        std::vector<IrcClient*> vect = channel->getOperators();
        std::string channel_name = channel->getName();
        channel_name.erase(0, 1);
        for (std::vector<IrcClient*>::iterator it = vect.begin(); it != vect.end(); it++) {
            client.sendResponse("352 " + client.getNickname() + " " + channel_name + " " + (*it)->getUsername() + \
            " * ft_irc " + (*it)->getNickname() + " G :0 " + (*it)->getRealname());
        } 
    }

    // End of response
    client.sendResponse("315 * :" + client.getNickname());
}

void PartCommand::execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args)
{
    if (args.size() < 1) {
        client.sendResponse("461 " + client.getNickname() + " PART");
        return;
    }

    if (server.channelExists(args[0])) {
        try
        {
            server.channelHasClient(args[0], client);
        }
        catch (std::runtime_error &e)
        {
            client.sendResponse("442 " + client.getNickname() + " " + args[0] + " :You're not on that channel"); // 442: ERR_NOTONCHANNEL
            return ;
        }
    } else {
        client.sendResponse("403 " + client.getNickname() + " " + args[0] + " :No such channel"); // 403: ERR_NOSUCHCHANNEL
        return ;
    }

    if (args.size() == 1) {
        server.kickClientFromChannel(client, args[0]);
        client.sendResponse(":" + client.getNickname() + "!~" + client.getUsername() + "@" + client.getHostname() + " PART " + args[0] + " :No reason");
    } else {
        server.kickClientFromChannel(client, args[0]);
        std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + client.getHostname() + " PART " + args[0] + " " + args[1];
        for (size_t i = 2; i < args.size(); i++)
            response += " " + args[i];
        client.sendResponse(response);
    }
}

// void ListCommand::execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) {
//     if (args.size() == 0) {
//         client.sendResponse("321 " + client.getNickname() + " Channel :Users Name");
//         std::vector<IrcChannel> list = server.getChannels();
//         for (size_t i = 0; i < list.size(); i++)
//         {
//             std::stringstream ss;
//             ss << list[i].getNumClients();
//             std::string num = ss.str();
//             client.sendResponse(":ft_irc 322 " + client.getNickname() + " " + list[i].getName() + " " + num + " :" + list[i].getTopic());
//         } 
//         client.sendResponse("323 " + client.getNickname() + " :End of /LIST");
//     }
// }

void ListCommand::execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) {
    if (args.size() == 0) {
        std::string server_name = server.getHostname();
        client.sendResponse(":" + server_name + " 321 " + client.getNickname() + " Channel :Users Name");
        std::vector<IrcChannel> list = server.getChannels();
        for (size_t i = 0; i < list.size(); i++)
        {
            std::stringstream ss;
            ss << list[i].getNumClients();
            std::string num = ss.str();
            client.sendResponse(":" + server_name + " 322 " + client.getNickname() + " " + list[i].getName() + " " + num + " :" + list[i].getTopic());
        } 
        client.sendResponse(":" + server_name + " 323 " + client.getNickname() + " :End of channel list.");
    }
}


// Implementation of ModeCommand::execute
void ModeCommand::execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) {
    if (args.size() < 1) {
        // Handle incorrect usage: not enough arguments
        // You may want to send an appropriate error message to the client
        return;
    }

    const std::string& channelName = args[0];

    // Locate the channel
    std::vector<IrcChannel>& channels = server.getChannels();
    std::vector<IrcChannel>::iterator channelIt = std::find_if(channels.begin(), channels.end(), ChannelNameMatcher(channelName));

    if (channelIt == channels.end()) {
        // Handle channel not found
        // You may want to send an appropriate error message to the client
        return;
    }
    // Check if the client has the necessary privileges to set the mode (usually the channel operator)
    // This step depends on your IrcClient and IrcChannel implementation

    // If there's only a channel name, send the mode response
    if (args.size() == 1) {
        std::string modeString = channelIt->getMode();
        std::string hostname = server.getHostname();
        // std::string timestamp = "1679504576";
        std::time_t current_time = std::time(NULL);

        client.sendResponse(":" + hostname + " 324 " + client.getNickname() + " " + channelName + " :" + modeString);
        client.sendResponse(":" + hostname + " 329 " + client.getNickname() + " " + channelName + " :" + std::to_string(current_time));
    } else if (channelIt->isOperator(client)) {
        const std::string& mode = args[1];
        bool add = mode[0] == '+';
        // Set or unset the mode
        if (add) {
            channelIt->addMode(mode.substr(1));
        } else {
            channelIt->removeMode(mode.substr(1));
        }
    } else if (args[1][0] == '+' || args[1][0] == '-'){
        // Handle insufficient privileges
        // You may want to send an appropriate error message to the client
        // :penguin.omega.example.org 482 bob #general :You must be a channel op or higher to set channel mode p (private).
        client.sendResponse("482 " + client.getNickname() + " " + channelName + " :You must be a channel op or higher to set channel mode " + args[1]);
    } else {
        // Banning is not handled here
    }
}

void TopicCommand::execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) {
    if (args.size() < 1) {
        // Send an error message to the client.
        client.sendResponse("461 " + client.getNickname() + " TOPIC"); // 461: ERR_NEEDMOREPARAMS
        return;
    }

    if (!server.channelExists(args[0])) {
        client.sendResponse("403 " + client.getNickname() + " " + args[0] + " :No such channel"); // 403: ERR_NOSUCHCHANNEL
        return ;
    }

    IrcChannel *channel = server.getChannelByName(args[0]);
    if (args.size() == 1) {
        client.sendResponse("332 " + client.getNickname() + " " + args[0] + " :" + channel->getTopic());
    } else {
        // Find the colon (:) to parse the message.
        size_t colonPos = args[1].find(':');
        std::string topic;

        if (colonPos != 0) {
            topic = args[1];
        } else {
            topic = args[1].substr(colonPos + 1);
            for (size_t i = 2; i < args.size(); ++i) {
                topic += " " + args[i];
            }
        }

        // std::string topic = args[1];
        // for (size_t i = 2; i < args.size(); i++)
        //     topic += " " + args[i];
        channel->setTopic(topic);
        client.sendResponse("332 " + client.getNickname() + " " + args[0] + " :" + topic); // SET TOPIC
    }
}

void InviteCommand::execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) {
        if (args.size() != 2) {
            client.sendResponse("461 " + client.getNickname() + "INVITE :Not enough parameters\r\n"); 
            return;
        }
        std::string user = args[0];
        std::string channel = args[1];
        
        server.inviteUserToChannel(user, channel);
}