#include "Command.hpp"
#include "Server/IrcServ.hpp"
#include "Server/IrcClient.hpp"
#include <sstream>

void PassCommand::execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) {
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

    if (args.size() < 1) {
        client.sendResponse("461 PASS :Not enough parameters"); // 461: ERR_NEEDMOREPARAMS
        return;
    }
    
    if (!client.getIsAuth()) {
        if (password == server.getPass()) {
            client.setAuth();
        } else {
            client.sendResponse("464 " + client.getNickname() + " :Password incorrect " + password + " END"); // 464: ERR_PASSWDMISMATCH
        }
    } else {
        client.sendResponse("462 " + client.getNickname() + " :You may not reregister"); // 462: ERR_ALREADYREGISTRED
    }
}


void NickCommand::execute(IrcServ&, IrcClient& client, const std::vector<std::string>& args) {
	if (args.size() != 1) {
        client.sendResponse("Usage: NICK <nickname>");
        return;
    }

    // Set the client's nickname and send a response.
    client.setNickname(args[0]);
	// client.sendResponse("Your nickname is now " + args[0]);
    if (client.getUserAndNickSet()) {
        client.sendResponse("001 " + client.getNickname() + " :Welcome to the Internet Relay Network " + client.getNickname()); // 001: RPL_WELCOME
        client.sendResponse("002 " + client.getNickname() + " :Your host is " + client.getServername() + ", running version 1.0"); // 002: RPL_YOURHOST
    }
}

void QuitCommand::execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) {
    (void)args;
    server.handleDisconnect(client.getFd());
}

// void UserCommand::execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) {
//     if (args.size() != 4) {
//         // Send an error message to the client.
//         return;
//     }
//     // Set the client's username, real name, and other information.
// }
void UserCommand::execute(IrcServ&, IrcClient& client, const std::vector<std::string>& args) {
    if (args.size() < 4) {
        client.sendResponse("Usage: USER <username> <hostname> <servername> <realname>");
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
        client.sendResponse("001 " + client.getNickname() + " :Welcome to the Internet Relay Network " + client.getNickname()); // 001: RPL_WELCOME
        client.sendResponse("002 " + client.getNickname() + " :Your host is " + servername + ", running version 1.0"); // 002: RPL_YOURHOST
    }

    // Sending your host message
    // client.sendResponse("Your host is " + servername + ", running version 1.0");
}



void JoinCommand::execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) {
    if (args.size() != 1) {
        // Send an error message to the client.
        return;
    }
    // Make the client join the specified channel.
    server.joinChannel(client, args[0]);
}

void PrivmsgCommand::execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        // Send an error message to the client.
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
    server.sendMessage(client, args[0], message);
}

void NoticeCommand::execute(IrcServ& ircServ, IrcClient& client, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        client.sendResponse("461 :Not enough arguments for NOTICE command");
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

    // const std::string& target = args[0];
    // const std::string& message = args[1];

    // ircServ.sendMessage(client, target, message);
        // Send the message from the client to the specified target.
    ircServ.sendMessage(client, args[0], message);
}

// // Implement other command classes like PartCommand, QuitCommand, etc.


// // Implement other command classes like PartCommand, QuitCommand, etc.
