#include "Command.hpp"
#include "Server/IrcServ.hpp"
#include "Server/IrcClient.hpp"
#include <sstream>

void PassCommand::execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) {
    if (args.size() != 1) {
        client.sendResponse("Invalid number of arguments for PASS command");
        return;
    }
    
    if (!client.getIsAuth()) {
        if (args[0] == server.getPass()) {
            client.sendResponse("Congratulations! You are authenticated!");
            client.setAuth();
        } else {
            client.sendResponse("464 * :It's a wrong password ;(");
        }
    } else {
        client.sendResponse("You are already authenticated.");
    }
}

void NickCommand::execute(IrcServ&, IrcClient& client, const std::vector<std::string>& args) {
	if (args.size() != 1) {
        client.sendResponse("Usage: NICK <nickname>");
        return;
    }

    // Set the client's nickname and send a response.
    client.setNickname(args[0]);
	client.sendResponse("Your nickname is now " + args[0]);
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
    if (args.size() != 4) {
        client.sendResponse("Usage: USER <username> <hostname> <servername> <realname>");
        return;
    }
    
    std::string username = args[0];
    std::string hostname = args[1];
    std::string servername = args[2];
    std::string realname = args[3];

    // Set the client's username, real name, and other information.
    client.setUsername(username);
    client.setHostname(hostname);
    client.setServername(servername);
    client.setRealname(realname);

    // Sending a welcome message
	client.sendResponse("Welcome to the Internet Relay Network " + username + "!");

	// Sending your host message
	client.sendResponse("Your host is " + servername + ", running version 1.0");


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

    // Concatenate all arguments after the first one as the message.
    std::ostringstream messageStream;
    for (size_t i = 1; i < args.size(); ++i) {
        messageStream << args[i];
        if (i < args.size() - 1) {
            messageStream << " ";
        }
    }
    std::string message = messageStream.str();

    // Send a private message from the client to the specified target.
    server.sendPrivateMessage(client, args[0], message);
}

// // Implement other command classes like PartCommand, QuitCommand, etc.
