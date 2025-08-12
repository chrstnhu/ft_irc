#include "../../includes/commands/PASS.hpp"

PASS::PASS() : Command("PASS", 1, false) {}

PASS::~PASS() {}

// Execute | Format: PASS <password>
void PASS::execute(Server* server, Client* client, IRCMessage message, std::vector<execReturnData>& execReturn) {
    execReturnData returnData = server->createBasicExecReturnData(client->getFd());

    if (message.params.empty()) {
        returnData.message = ERR_NEEDMOREPARAMS(client->getNick(), "PASS");
        execReturn.push_back(returnData);
        return;
    }

    std::string password = message.params[0];

    if (!client->isPasswordNeeded()) {
        returnData.message = ERR_ALREADYREGISTERED(client->getNick());
        execReturn.push_back(returnData);
        return;
    }

    if (server->getPassword() != password) {
        returnData.message = ERR_PASSWDMISMATCH(client->getNick());
        execReturn.push_back(returnData);
        return;
    }

    client->setIsPasswordNeeded(false);
}