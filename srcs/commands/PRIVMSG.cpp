#include "../../includes/commands/PRIVMSG.hpp"
#include "../../includes/bonus/BotBonus.hpp"

PRIVMSG::PRIVMSG(void) : Command("PRIVMSG", 0, true) {}

PRIVMSG::~PRIVMSG(void) {}

// Execute [Format: PRIVMSG <destination(user/channel)> <msg>]
void PRIVMSG::execute(Server* server, Client* client, IRCMessage message, std::vector<execReturnData>& execReturn) {
    execReturnData returnData = server->createBasicExecReturnData(client->getFd());
    std::string msg = message.text;

    if (msg.empty()) {
        returnData.message = ERR_NOTEXTTOSEND(client->getNick());
        execReturn.push_back(returnData);
        printInfo("PRIVMSG failed: No text to send by " + client->getNick());
        return;
    }
    if (message.params.empty()) {
        returnData.message = ERR_NORECIPIENT(client->getNick());
        execReturn.push_back(returnData);
        printInfo("PRIVMSG failed: No recipient by " + client->getNick());
        return;
    }


    std::vector<std::string> wantedChannels = split(message.params[0], ',');
    bool hasErrors = false;
    Client* bot = server->getBotManager().getBot();

    for (std::vector<std::string>::iterator it = wantedChannels.begin(); it != wantedChannels.end(); ++it) {
        std::string destination = *it;
        std::string target = destination;

        if (destination[0] == '#') {
            destination = destination.substr(1);
            target = "#" + destination;
            Channel* channel = server->getChannelByName(destination);
            if (!channel) {
                returnData.message += ERR_NOSUCHNICK(client->getNick(), target) + CRLF;
                hasErrors = true;
                continue;
            }
            if (!channel->hasJoined(client->getFd())) {
                returnData.message += ERR_NOTONCHANNEL(client->getNick(), target) + CRLF;
                hasErrors = true;
                continue;
            }

            std::string privMsg = ":" + client->getNick() + "!" + client->getUsername() + "@" + client->getHostname() +
                                " PRIVMSG " + target + " :" + msg + "\r\n";
            std::set<int> joinedFds = channel->getJoinedClientsFds();
            for (std::set<int>::iterator fdIt = joinedFds.begin(); fdIt != joinedFds.end(); ++fdIt) {
                if (*fdIt != client->getFd()) {
                    Client* channelClient = server->getClientByFd(*fdIt);
                    if (channelClient) {
                        channelClient->getOutdata() += privMsg;
                    }
                }
            }
            printInfo("PRIVMSG sent to #" + destination + " by " + client->getNick());
            
            if (bot) {
                server->getBotManager().runBotCommand(client, channel, msg);
            }

        } else {
            Client* destClient = server->getClientByNick(destination);
            if (!destClient) {
                returnData.message += ERR_NOSUCHNICK(client->getNick(), destination) + CRLF;
                hasErrors = true;
                continue;
            }

            std::string privMsg = ":" + client->getNick() + "!" + client->getUsername() + "@" + client->getHostname() +
                                " PRIVMSG " + destination + " :" + msg + "\r\n";
            destClient->getOutdata() += privMsg;
            printInfo("PRIVMSG sent to " + destination + " by " + client->getNick());
        }
    }

    if (hasErrors) {
        execReturn.push_back(returnData);
    }
}