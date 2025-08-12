#include "../includes/Channel.hpp"

const char Channel::VALID_MODES[] = {'i', 't', 'k', 'o', 'l'};

/**
 * Channel constructor
 * @param name: The name of the channel to be created.
 * @param server: A pointer to the server where the channel is being created.
 * @param client: A pointer to the client who is creating the channel.
 */
Channel::Channel(std::string name, Server* server, Client* client) : _name(name), _maxUsers(0), _server(server) {
    if (name.empty() || name.length() > MAX_CHANNEL_NAME_LENGTH ||  
        name.find_first_of(" \t\r\n,") != std::string::npos)
	{
        printInfo("Enter invalid channel name" + name);
        throw std::runtime_error("Invalid channel name: " + name);
    }
    int clientFd = client->getFd();
    if (clientFd <= 0 || !server->getClientByFd(clientFd)) {
        throw std::runtime_error("Invalid client for channel creation");
    }
    _joinedClientsFds.insert(clientFd);
    _operatorsClientsFds.insert(clientFd);

    client->joinChannel(name);
    printInfo(name + " channel created successfully!");
}

Channel::~Channel() {
}

// Getters
std::string const& Channel::getName() const { 
    return _name; 
}

std::string const& Channel::getTopic() const { 
    return _topic; 
}

std::string const& Channel::getPassword() const { 
    return _password; 
}

std::vector<char>& Channel::getModes() { 
    return _modes; 
}

int const& Channel::getMaxUsers() const { 
    return _maxUsers; 
}

std::set<int>& Channel::getJoinedClientsFds() { 
    return _joinedClientsFds; 
}

std::set<int>& Channel::getOperatorsClientsFds() { 
    return _operatorsClientsFds; 
}

std::set<int>& Channel::getInvitedClientsFds() { 
    return _invitedClientsFds; 
}

Server* Channel::getServer() const { 
    return _server; 
}

int Channel::getJoinedClientsCount() const { 
    return static_cast<int>(_joinedClientsFds.size()); 
}

// Setters
void Channel::setTopic(const std::string& topic) {
    _topic = topic;
}

void Channel::setPassword(const std::string& password) {
    _password = password;
}

void Channel::setMaxUsers(int maxUsers) {
    _maxUsers = maxUsers;
}

/**
 * Add/remove a mode to the channel if it is valid and not already present.
 * @param mode: The mode character to be added.
 */
void Channel::addMode(char mode) {
    if (std::find(VALID_MODES, VALID_MODES + 5, mode) == VALID_MODES + 5) {
        return;
    }
    if (std::find(_modes.begin(), _modes.end(), mode) == _modes.end()) {
        _modes.push_back(mode);
    }
}

void Channel::removeMode(char mode) {
    std::vector<char>::iterator it = std::find(_modes.begin(), _modes.end(), mode);
    if (it != _modes.end()) {
        _modes.erase(it);
    }
}

bool Channel::hasMode(char mode) {
    return std::find(_modes.begin(), _modes.end(), mode) != _modes.end();
}

/*
 * Grant or ungrant operator status to a client in the channel.
 * @param clientFd: The file descriptor of the client to be granted or ungranted.
 */
void Channel::grantOperator(int clientFd) {
    if (_joinedClientsFds.find(clientFd) != _joinedClientsFds.end()) {
        _operatorsClientsFds.insert(clientFd);
    }
}

void Channel::ungrantOperator(int clientFd) {
    if (_operatorsClientsFds.find(clientFd) != _operatorsClientsFds.end()) {
        _operatorsClientsFds.erase(clientFd);
    }
}

bool Channel::isOperator(int clientFd) {
    return _operatorsClientsFds.find(clientFd) != _operatorsClientsFds.end();
}

/** 
 * Join a client to the channel, if the channel is not full and the client is valid.
 * @param clientFd: The file descriptor of the client to be joined.
 */
void Channel::join(int clientFd) {
    Server* server = this->getServer();
    Client* client = server->getClientByFd(clientFd);
    if (!client) {
        printWarning("Invalid client FD " + numberToString(clientFd) + " for join");
        return;
    }
    if (_maxUsers > 0 && static_cast<int>(_joinedClientsFds.size()) >= _maxUsers) {
        return;
    }
    _joinedClientsFds.insert(clientFd);
    client->joinChannel(this->getName());
}

bool Channel::hasJoined(int clientFd) {
    return _joinedClientsFds.find(clientFd) != _joinedClientsFds.end();
}

/**
 *  Part a client from the channel, if they are joined.
 * If the channel becomes empty, it is removed from the server.
 * @param clientFd: The file descriptor of the client to be parted.
*/
void Channel::part(int clientFd) {
    std::set<int>::iterator it = _joinedClientsFds.find(clientFd);
    if (it != _joinedClientsFds.end()) {
        Server* server = this->getServer();
        Client* client = server->getClientByFd(clientFd);
        if (client)
            client->partChannel(this->getName());
        _joinedClientsFds.erase(it);
        _operatorsClientsFds.erase(clientFd);
        _invitedClientsFds.erase(clientFd);
        if (_joinedClientsFds.empty()) {
            std::string name = this->getName();
            server->rmChannel(name);
            printInfo(name + " channel has been removed! (No more clients inside)");
        }
    }
}

/**
 * Invite/uninvite a client to the channel.
 * If the client is already invited, it does nothing.
 * @param clientFd: The file descriptor of the client to be invited.
 */
void Channel::invite(int clientFd) {
    Client* client = _server->getClientByFd(clientFd);
    if (!client) {
        printWarning("Invalid client FD " + numberToString(clientFd) + " for invite");
        return;
    }
    _invitedClientsFds.insert(clientFd);
}

void Channel::uninvite(int clientFd) {
    _invitedClientsFds.erase(clientFd);
}

bool Channel::isInvited(int clientFd) {
    return _invitedClientsFds.find(clientFd) != _invitedClientsFds.end();
}

/**
 * Generate join messages for a client when they join the channel.
 * It includes the welcome message, topic, and a list of nicknames in the channel.
 * @param client: The client who is joining the channel.
 * @return A string containing the join messages.
 */
std::string Channel::generateJoinMessages(Client* client)
{
    if (!client) {
        printWarning("Null client in generateJoinMessages for channel " + _name);
        return "";
    }

    std::ostringstream infos;
    infos << RPL_JOIN(client->getFullUserId(), _name) << CRLF;
    if (!_topic.empty())
        infos << RPL_TOPIC(client->getNick(), _name, _topic) << CRLF;

    std::string nicknamesList;
    for (std::set<int>::iterator it = _joinedClientsFds.begin(); it != _joinedClientsFds.end(); ++it) {
        Client* joinedClient = _server->getClientByFd(*it);
        if (joinedClient) {
            if (isOperator(*it))
                nicknamesList += "@";
            nicknamesList += joinedClient->getNick();
            std::set<int>::iterator next = it;
            if (++next != _joinedClientsFds.end())
                nicknamesList += " ";
        }
    }

    infos << RPL_NAMREPLY(client->getNick(), _name, nicknamesList) << CRLF
          << RPL_ENDOFNAMES(client->getNick(), _name) << CRLF;
    return infos.str();
}

/**
 * Broadcast a message to all clients in the channel.
 * The message is sent to each client's outdata.
 * @param message: The message to be broadcasted.
 */
void Channel::broadcastMessage(const std::string& message) {
    for (std::set<int>::iterator it = _joinedClientsFds.begin(); it != _joinedClientsFds.end(); ++it) {
        Client* client = _server->getClientByFd(*it);
        if (client)
            client->getOutdata() += message;
    }
}