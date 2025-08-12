#include "../../includes/commands/KICK.hpp"

KICK::KICK(void) : Command("KICK", 2, true) {}

KICK::~KICK(void) {}

// Execute | Format: KICK <channel> <nick> (reason)]
void KICK::execute(Server* server, Client* client, IRCMessage message, std::vector<execReturnData>& execReturn) {
    execReturnData returnData = server->createBasicExecReturnData(client->getFd());

    if (message.params.empty() || message.params.size() < 2) {
        returnData.message = ERR_NEEDMOREPARAMS(client->getNick(), "KICK") + CRLF;
        execReturn.push_back(returnData);
        return;
    }

    std::string reason = "No reason given";
    if (!message.text.empty()) {
        reason = message.text;
    }

    std::vector<std::string> channels = split(message.params[0], ',');
    std::string targetNick = message.params[1];

    for (size_t i = 0; i < channels.size(); ++i) {
        std::string channelName = channels[i];
        if (channelName.empty() || channelName[0] != '#') {
            returnData.message += ERR_BADCHANMASK(channelName) + CRLF;
        } else {
            channelName = channelName.substr(1);
            Channel* channel = server->getChannelByName(channelName);
            Client* clientToKick = server->getClientByNick(targetNick);
            if (!channel) {
                returnData.message += ERR_NOSUCHCHANNEL(client->getNick(), channelName) + CRLF;
            } else if (!channel->hasJoined(client->getFd())) {
                returnData.message += ERR_NOTONCHANNEL(client->getNick(), channelName) + CRLF;
            } else if (!channel->isOperator(client->getFd())) {
                returnData.message += ERR_CHANOPRIVSNEEDED(client->getNick(), channelName) + CRLF;
            } else if (!clientToKick || !server->isValidNickname(targetNick)) {
                returnData.message += ERR_NOSUCHNICK(client->getNick(), targetNick) + CRLF;
            } else if (!clientToKick || !channel->hasJoined(clientToKick->getFd())) {
                returnData.message += ERR_USERNOTINCHANNEL(client->getNick(), targetNick, channelName) + CRLF;
            } else if (clientToKick->getFd() == client->getFd()) {
                returnData.message += ERR_CANTKICKSELF(client->getFullUserId(), client->getNick(), channelName) + CRLF;
            } else {
                channel->broadcastMessage(RPL_KICK(client->getFullUserId(), channelName, clientToKick->getNick(), reason) + CRLF);
                channel->ungrantOperator(clientToKick->getFd());
                channel->part(clientToKick->getFd());
            }
        }
    }
    if (!returnData.message.empty()) {
        execReturn.push_back(returnData);
    }
}
