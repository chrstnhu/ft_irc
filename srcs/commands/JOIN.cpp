#include "../../includes/commands/JOIN.hpp"
#include "../../includes/Channel.hpp"

JOIN::JOIN(void) : Command("JOIN", 1, true) {}

JOIN::~JOIN(void) {}

// Convert a string to a vector of tokens based on a delimiter
static std::map<std::string, std::string> createMap(std::string const& str1, std::string const& str2) {
    std::map<std::string, std::string> result;
    std::vector<std::string> tokens1 = split(str1, ',');
    std::vector<std::string> tokens2 = split(str2, ',');

    if (tokens1.empty()) {
        return result;
    } else if (tokens2.empty()) {
        tokens2.push_back("");
    }

    for (size_t i = 0; i < tokens1.size(); i++) {
        std::string key = tokens1[i];
        std::string value = (i < tokens2.size()) ? tokens2[i] : tokens2.back();
        result.insert(std::pair<std::string, std::string>(key, value));
    }
    
    return result;
}

// Execute | Format: JOIN <channel>{,<channel>} [<key>{,<key>}
void JOIN::execute(Server* server, Client* client, IRCMessage message, std::vector<execReturnData>& execReturn) {
    if (message.params.empty())
        return;

    std::string channels = message.params[0];
    std::string keys = message.params.size() > 1 ? message.params[1] : "";

    std::map<std::string, std::string> channelsMap = createMap(channels, keys);
    std::map<std::string, Channel>& serverChannels = server->getChannels();

    for (std::map<std::string, std::string>::iterator it = channelsMap.begin(); it != channelsMap.end(); ++it) {
        std::string channelName = it->first;
        std::string key = it->second;

        if (channelName.empty() || channelName[0] != '#') {
            continue;
        }

        channelName = channelName.substr(1);
        execReturnData returnData = server->createBasicExecReturnData(client->getFd());

        if (!server->isValidChannelName("#" + channelName)) {
            returnData.message = ERR_BADCHANMASK(channelName) + CRLF;
            execReturn.push_back(returnData);
            continue;
        }

        Channel* selectedChannel = server->getChannelByName(channelName);
        if (selectedChannel) {
            if (selectedChannel->hasMode(CHANNEL_HAS_PASSWORD) && selectedChannel->getPassword() != key) {
                returnData.message = ERR_BADCHANNELKEY(client->getNick(), channelName) + CRLF;
            } else if (selectedChannel->hasMode(CHANNEL_IS_USER_LIMITED) &&
                    selectedChannel->getJoinedClientsCount() >= selectedChannel->getMaxUsers()) {
                returnData.message = ERR_CHANNELISFULL(client->getNick(), channelName) + CRLF;
            } else if (selectedChannel->hasMode(INVITE_ONLY_CHANNEL) &&
                    !selectedChannel->isInvited(client->getFd())) {
                returnData.message = ERR_INVITEONLYCHAN(client->getNick(), channelName) + CRLF;
            } else {
                selectedChannel->join(client->getFd());
                selectedChannel->uninvite(client->getFd());

                std::string joinMsg = selectedChannel->generateJoinMessages(client);
				std::set<int>& clientFds = selectedChannel->getJoinedClientsFds();
    			std::set<int>::iterator itFd;
    			for (itFd = clientFds.begin(); itFd != clientFds.end(); ++itFd)
				{
        	    	Client* c = server->getClientByFd(*itFd);
    				if (c)
        			c->getOutdata() += joinMsg;
				}
                continue;
            }
        } else {
            Channel newChannel(channelName, server, client);
            serverChannels.insert(std::make_pair(channelName, newChannel));
            returnData.message = newChannel.generateJoinMessages(client);
        }

        if (!returnData.message.empty()) {
            execReturn.push_back(returnData);
        }
    }
}