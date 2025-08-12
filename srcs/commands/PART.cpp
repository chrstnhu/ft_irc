#include "../../includes/commands/PART.hpp"

PART::PART(void) : Command("PART", 1, true) {}

PART::~PART(void) {}

// Execute | Format: PART <channel>{,<channel>} [<reason>]
void PART::execute(Server* server, Client* client, IRCMessage message, std::vector<execReturnData>& execReturn) {
    execReturnData returnData = server->createBasicExecReturnData(client->getFd());

    if (message.params.empty()) {
        returnData.message = ERR_NEEDMOREPARAMS(client->getNick(), "PART") + CRLF;
        execReturn.push_back(returnData);
        return;
    }
    std::string reason = "No reason given";
    if (!message.text.empty()) {
        reason = message.text;
    }
    std::vector<std::string> channelsToPart = split(message.params[0], ',');
    for (std::vector<std::string>::iterator it = channelsToPart.begin() ; it != channelsToPart.end(); ++it) {
        std::string channelName = *it;
        if (channelName.empty() || channelName[0] != '#') {
            returnData.message += ERR_BADCHANMASK(channelName) + CRLF;
        } else {
            channelName = channelName.substr(1);
            Channel* currentChannel = server->getChannelByName(channelName);

            if (!currentChannel) {
                returnData.message += ERR_NOSUCHCHANNEL(client->getNick(), channelName) + CRLF;
            } else if (!currentChannel->hasJoined(client->getFd())) {
                returnData.message += ERR_NOTONCHANNEL(client->getNick(), channelName) + CRLF;
            } else {
                currentChannel->broadcastMessage(RPL_PART(client->getFullUserId(), channelName, reason) + CRLF);
                currentChannel->ungrantOperator(client->getFd());
                currentChannel->part(client->getFd());
            }
        }
    }

    if (!returnData.message.empty()) {
        execReturn.push_back(returnData);
    }
}