#include "../../includes/commands/PING.hpp"

PING::PING() : Command("PING", 1, false) {}

PING::~PING() {}

// Execute | Format: PING <token>
void PING::execute(Server* server, Client* client, IRCMessage message, std::vector<execReturnData>& execReturn) {
    if (message.params.empty()) {
        printWarning("PING command received without parameter from client " + client->getNick());
        return;
    }

    execReturnData pongReply = server->createBasicExecReturnData(client->getFd());
    pongReply.message = RPL_PONG(client->getFullUserId(), message.params[0]);
    execReturn.push_back(pongReply);
}
