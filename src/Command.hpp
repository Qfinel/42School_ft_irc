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

class QuitCommand : public Command {
public:
    void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class UserCommand : public Command {
public:
    void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class JoinCommand : public Command {
public:
    void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class PrivmsgCommand : public Command {
public:
    void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class NoticeCommand : public Command {
public:
    virtual void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class PingCommand : public Command {
public:
    virtual void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class KickCommand : public Command {
public:
    void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

// // Add other command classes like PartCommand, QuitCommand, etc.
