#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include "Server.hpp"
# include "Client.hpp"

class Server;
class Client;

class Channel
{
private:
    std::string _name;
    std::string _topic;
    std::string _password;
    std::vector<char> _modes;
    int _maxUsers;
    std::set<int> _joinedClientsFds;
    std::set<int> _operatorsClientsFds;
    std::set<int> _invitedClientsFds;
    Server* _server;

    static const size_t MAX_CHANNEL_NAME_LENGTH = 50;
    static const char VALID_MODES[];

public:
    Channel(std::string name, Server* server, Client* client);
    ~Channel();

    // GettersJOIN
    std::string const& getName() const;
    std::string const& getTopic() const;
    std::string const& getPassword() const;
    std::vector<char>& getModes();
    int const& getMaxUsers() const;
    std::set<int>& getJoinedClientsFds();
    std::set<int>& getOperatorsClientsFds();
    std::set<int>& getInvitedClientsFds();
    Server* getServer() const;

    // Methods
    std::string generateJoinMessages(Client* client);
    bool isOperator(int clientFd);
    bool hasJoined(int clientFd);
    bool isInvited(int clientFd);
    bool hasMode(char mode);
    int getJoinedClientsCount() const;

    // Setters
    void setTopic(const std::string& topic);
    void setPassword(const std::string& password);
    void addMode(char mode);
    void removeMode(char mode);
    void setMaxUsers(int maxUsers);
    void grantOperator(int clientFd);
    void ungrantOperator(int clientFd);
    void join(int clientFd);
    void part(int clientFd);
    void invite(int clientFd);
    void uninvite(int clientFd);
    void broadcastMessage(const std::string& message);
};

#endif