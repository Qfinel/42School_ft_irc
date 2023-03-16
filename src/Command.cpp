#include "Command.hpp"
#include "Server/IrcServ.hpp"
#include "Server/IrcClient.hpp"

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
            client.sendResponse("464 :It's a wrong password ;(");
        }
    } else {
        client.sendResponse("You are already authenticated.");
    }
}

void NickCommand::execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) {
    (void)server;
	if (args.size() != 1) {
        // Send an error message to the client.
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

// void JoinCommand::execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) {
//     if (args.size() != 1) {
//         // Send an error message to the client.
//         return;
//     }
//     // Make the client join the specified channel.
//     server.joinChannel(client, args[0]);
// }

// void PrivmsgCommand::execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) {
//     if (args.size() < 2) {
//         // Send an error message to the client.
//         return;
//     }
//     // Send a private message from the client to the specified target.
//     server.sendPrivateMessage(client, args[0], args[1]);
// }

// // Implement other command classes like PartCommand, QuitCommand, etc.
