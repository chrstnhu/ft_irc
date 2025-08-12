#include "../../includes/commands/USER.hpp"

USER::USER(void) : Command("USER", 1, false) {}

USER::~USER(void) {}

// Execute | Format: USER <username> <hostname> <servername> :<realname>
void USER::execute(Server* server, Client* client, IRCMessage message, std::vector<execReturnData>& execReturn) {
    execReturnData returnData = server->createBasicExecReturnData(client->getFd());

    if (message.params.size() < 3 || message.text.empty()) {
        returnData.message = ERR_NEEDMOREPARAMS(client->getNick(), this->getName());
        execReturn.push_back(returnData);
        return;
    }

    if (client->isLoggedIn()) {
        returnData.message = ERR_ALREADYREGISTERED(client->getNick());
        execReturn.push_back(returnData);
        return;
    }

    client->setUsername(message.params[0]);
    if (message.params[1].empty() || (message.params[1] == client->getUsername())) {
        client->setHostname("localhost");
    } else {
        client->setHostname(message.params[1]);
    }
    if (message.params[2].empty() || (message.params[2] == client->getHostname())) {
        client->setServername("ft_irc");
    } else {
        client->setServername(message.params[2]);
    }
    client->setRealname(message.text);
}