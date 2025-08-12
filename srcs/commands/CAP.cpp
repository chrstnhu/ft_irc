#include "../../includes/commands/CAP.hpp"

CAP::CAP(void) : Command("CAP", 0, false) {
    _capabilitiesList.push_back("NO CAPABILITIES");
    _capabilitiesList.push_back("multi-prefix");
    _capabilitiesList.push_back("sasl");
    _capabilitiesList.push_back("away-notify");
}

CAP::~CAP(void) {}

// Convert a vector of strings
std::string CAP::vecToStr(std::vector<std::string> vec) {
    std::string result;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i != 0) {
            result += ", ";
        }
        result += vec[i];
    }
    return result;
}

// Execute | Format: CAP <command> <(params)>
void CAP::execute(Server* server, Client* client, IRCMessage message, std::vector<execReturnData>& execReturn) {
    execReturnData returnData = server->createBasicExecReturnData(client->getFd());

    if (message.params.empty()) {
        returnData.message = ERR_NEEDMOREPARAMS(client->getNick(), "CAP");
    } else {
        std::string command = message.params[0];

        if (command == "LS") {
            returnData.message = RPL_CAPABILITIES(client->getNick(), "LS", vecToStr(_capabilitiesList));
        } else if (command == "REQ") {
            if (message.params.size() < 2) {
                returnData.message = ERR_NEEDMOREPARAMS(client->getNick(), "CAP REQ");
            } else {
                std::string requestedCap = message.params[1];

                if (std::find(_capabilitiesList.begin(), _capabilitiesList.end(), requestedCap) != _capabilitiesList.end()) {
                    returnData.message = RPL_CAPABILITIES(client->getNick(), "ACK", requestedCap);
                } else {
                    returnData.message = RPL_CAPABILITIES(client->getNick(), "NAK", requestedCap);
                }
            }
        } else if (command == "END") {
            returnData.message = RPL_ENDCAP(client->getNick());
        } else {
            returnData.message = ERR_UNKNOWNCAP(client->getNick(), message.params[0]);
        }
    }
    execReturn.push_back(returnData);
}
