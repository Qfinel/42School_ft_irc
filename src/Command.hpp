#pragma once
#include <string>
#include <vector>

class IrcServ;

class IrcClient;

class Command {
public:
    virtual ~Command() {}
    virtual void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) = 0;
};

class PassCommand : public Command {
public:
    void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class NickCommand : public Command {
public:
    void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

// class UserCommand : public Command {
// public:
//     void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) override;
// };

// class JoinCommand : public Command {
// public:
//     void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) override;
// };

// class PrivmsgCommand : public Command {
// public:
//     void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) override;
// };

// // Add other command classes like PartCommand, QuitCommand, etc.
