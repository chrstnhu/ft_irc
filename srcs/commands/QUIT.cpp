#include "../../includes/commands/QUIT.hpp"

QUIT::QUIT(void) : Command("QUIT", 0, false) {}

QUIT::~QUIT(void) {}

// Execute | Format: QUIT <(reason)>
void QUIT::execute(Server* server, Client* client, IRCMessage message, std::vector<execReturnData>& execReturn) {
    (void)server;
    execReturnData returnData;
    
    std::vector<int> joinedChannelsClientsFds = client->getJoinedChannelsFds();
    std::string reason = "No reason given";
    if (!message.text.empty()) {
        reason = message.text;
    }

    returnData.clientsFds.insert(returnData.clientsFds.end(), joinedChannelsClientsFds.begin(), joinedChannelsClientsFds.end());
    if (std::find(returnData.clientsFds.begin(), returnData.clientsFds.end(), client->getFd()) == returnData.clientsFds.end()) {
        returnData.clientsFds.push_back(client->getFd());
    }

    returnData.message = RPL_QUIT(client->getFullUserId(), reason);

    client->setHasusedQuitCommand(true);
    execReturn.push_back(returnData);
}