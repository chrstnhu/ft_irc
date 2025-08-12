#include "../../includes/commands/TOPIC.hpp"

TOPIC::TOPIC(void) : Command("TOPIC", 1, true) {}

TOPIC::~TOPIC(void) {}

// Validate topic string
static bool isValidTopic(const std::string& topic) {
    return topic.length() <= MAX_TOPIC_LENGTH;
}

// Execute | Format: TOPIC <channel> [<topic>]
void TOPIC::execute(Server* server, Client* client, IRCMessage message, std::vector<execReturnData>& execReturn) {
    execReturnData returnData = server->createBasicExecReturnData(client->getFd());

    if (message.params.empty()) {
        returnData.message = ERR_NEEDMOREPARAMS(client->getNick(), "TOPIC");
        execReturn.push_back(returnData);
        return;
    }
    std::string channel = message.params[0];
    if (channel[0] != '#') {
        returnData.message = ERR_NOSUCHCHANNEL(client->getNick(), channel);
        execReturn.push_back(returnData);
        return;
    }
    channel = channel.substr(1);

    Channel* selectedChannel = server->getChannelByName(channel);
    if (!selectedChannel) {
        returnData.message = ERR_NOSUCHCHANNEL(client->getNick(), channel);
        execReturn.push_back(returnData);
        return;
    }
    if (!selectedChannel->hasJoined(client->getFd())) {
        returnData.message = ERR_NOTONCHANNEL(client->getNick(), channel);
        execReturn.push_back(returnData);
        return;
    }

    printInfo("TOPIC command for #" + channel + " by " + client->getNick());

    if (!message.hasText) {
        std::string topic = selectedChannel->getTopic();
        if (topic.empty()) {
            returnData.message = RPL_NOTOPIC(client->getNick(), channel);
        } else {
            returnData.message = RPL_TOPIC(client->getNick(), channel, topic);
        }
        printInfo("TOPIC query for #" + channel + ": " + (topic.empty() ? "No topic" : topic));
    } else {
        std::string newTopic = message.text;
        if (selectedChannel->hasMode(RESTRICTED_TOPIC_CHANNEL) && !selectedChannel->isOperator(client->getFd())) {
            returnData.message = ERR_CHANOPRIVSNEEDED(client->getNick(), channel);
        } else if (!isValidTopic(newTopic)) {
            returnData.message = ERR_INVALIDTOPIC(client->getFullUserId(), client->getNick(), channel);
        } else {
            selectedChannel->setTopic(newTopic);
            std::string topicMsg = ":" + client->getNick() + "!" + client->getUsername() + "@" + client->getHostname() +
                                " TOPIC #" + channel + " :" + newTopic + "\r\n";
            selectedChannel->broadcastMessage(topicMsg);
            printInfo("TOPIC changed for #" + channel + " to: " + newTopic);
        }
    }

    if (!returnData.message.empty()) {
        execReturn.push_back(returnData);
    }
}