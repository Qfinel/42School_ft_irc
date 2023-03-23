#pragma once
#include <string>
#include <vector>
#include "./Server/IrcChannel.hpp"

class IrcServ;

class IrcClient;

class Command {
protected:
    IrcServ& _server;
    class ChannelNameMatcher {
    public:
        ChannelNameMatcher(const std::string& channelName) : _channelName(channelName) {}

        bool operator()(const IrcChannel& channel) const {
            return channel.getName() == _channelName;
        }

    private:
        std::string _channelName;
    };
public:
    Command(IrcServ& server) : _server(server) {}
    virtual ~Command() {}
    virtual void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args) = 0;
    void sendResponseToChannel(IrcClient& sender, const std::string& channelName, const std::string& message);
};

class PassCommand : public Command {
public:
    PassCommand(IrcServ& server) : Command(server) {}
    void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class NickCommand : public Command {
public:
    NickCommand(IrcServ& server) : Command(server) {}
    void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class QuitCommand : public Command {
public:
    QuitCommand(IrcServ& server) : Command(server) {}
    void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class UserCommand : public Command {
public:
    UserCommand(IrcServ& server) : Command(server) {}
    void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class JoinCommand : public Command {
public:
    JoinCommand(IrcServ& server) : Command(server) {}
    void joinChannel(IrcClient &client, const std::string &channelName);
    void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class PrivmsgCommand : public Command {
public:
    PrivmsgCommand(IrcServ& server) : Command(server) {}
    void sendChannelMessage(IrcClient& sender, const std::string& channelName, const std::string& message);
    void sendPrivateMessage(IrcClient& sender, const std::string& targetUser, const std::string& message);
    void sendMessage(IrcClient& sender, const std::string& target, const std::string& message);
    void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class NoticeCommand : public Command {
public:
    NoticeCommand(IrcServ& server) : Command(server) {}
    void sendChannelMessage(IrcClient& sender, const std::string& channelName, const std::string& message);
    void sendPrivateMessage(IrcClient& sender, const std::string& targetUser, const std::string& message);
    void sendMessage(IrcClient& sender, const std::string& target, const std::string& message);
    virtual void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class PingCommand : public Command {
public:
    PingCommand(IrcServ& server) : Command(server) {}
    virtual void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class KickCommand : public Command {
public:
    KickCommand(IrcServ& server) : Command(server) {}
    void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class CapCommand : public Command {
public:
    CapCommand(IrcServ& server) : Command(server) {}
    void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class WhoCommand : public Command {
public:
    WhoCommand(IrcServ& server) : Command(server) {}
    void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class PartCommand : public Command {
public:
    PartCommand(IrcServ& server) : Command(server) {}
    void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class ListCommand : public Command {
public:
    ListCommand(IrcServ& server) : Command(server) {}
    void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class ModeCommand : public Command {
public:
    ModeCommand(IrcServ& server) : Command(server) {}
    void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class TopicCommand : public Command {
public:
    TopicCommand(IrcServ& server) : Command(server) {}
    void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

class InviteCommand : public Command {
public:
    InviteCommand(IrcServ& server) : Command(server) {}
    void execute(IrcServ& server, IrcClient& client, const std::vector<std::string>& args);
};

// // Add other command classes like PartCommand, QuitCommand, etc.
