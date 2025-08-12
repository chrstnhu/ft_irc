#include "../../includes/commands/INVITE.hpp"

INVITE::INVITE(void) : Command("INVITE", 2, true) {}

INVITE::~INVITE(void) {}

// Execute | Format: INVITE <nickname> <channel>
void INVITE::execute(Server* server, Client* client, IRCMessage message, std::vector<execReturnData>& execReturn) {
    execReturnData returnData = server->createBasicExecReturnData(client->getFd());

    if (message.params.size() < 2) {
        returnData.message = ERR_NEEDMOREPARAMS(client->getNick(), "INVITE") + CRLF;
        execReturn.push_back(returnData);
        return;
    }

    std::string targetNick = message.params[0];
    std::string channel = message.params[1];
    if (message.params[1].empty() || message.params[1][0] != '#') {
        returnData.message = ERR_BADCHANMASK(channel) + CRLF;
        execReturn.push_back(returnData);
        return;
    }

    channel = channel.substr(1);
    Channel* currentChannel = server->getChannelByName(channel);
    Client* clientToInvite = server->getClientByNick(targetNick);
    if (!currentChannel) {
        returnData.message = ERR_NOSUCHCHANNEL(client->getNick(), channel) + CRLF;
    } else if (!currentChannel->hasJoined(client->getFd())) {
        returnData.message = ERR_NOTONCHANNEL(client->getNick(), channel) + CRLF;
    } else if (!clientToInvite) {
        returnData.message = ERR_NOSUCHNICK(client->getNick(), targetNick) + CRLF;
    } else if (currentChannel->hasMode(INVITE_ONLY_CHANNEL) && !currentChannel->isOperator(client->getFd())) {
        returnData.message = ERR_CHANOPRIVSNEEDED(client->getNick(), channel) + CRLF;
    } else if (currentChannel->hasJoined(clientToInvite->getFd())) {
        returnData.message = ERR_USERONCHANNEL(client->getNick(), targetNick, channel) + CRLF;
    } else {
        std::string& outData = clientToInvite->getOutdata();
        outData += RPL_INVITE(client->getFullUserId(), client->getNick(), channel) + CRLF;
        
        returnData.message = RPL_INVITING(client->getNick(), targetNick, channel) + CRLF;
        currentChannel->invite(clientToInvite->getFd());
    }
    execReturn.push_back(returnData);
}
