#ifndef NICK_HPP
#define NICK_HPP

#include "../Command.hpp"

class NICK : public Command {
    public :
        NICK(void);
        virtual ~NICK(void);

        virtual void execute(Server* server, Client *client, 
                IRCMessage message, std::vector<execReturnData>&execReturn);
        bool isValidNickName(std::string nickname);
        bool isNickNameAlreadyUsed(std::string nickname, Server* server);
};

#endif