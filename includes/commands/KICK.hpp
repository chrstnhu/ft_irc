#ifndef KICK_HPP
#define KICK_HPP

#include "../Command.hpp"

class KICK : public Command {
    public :
        KICK(void);
        virtual ~KICK(void);

        virtual void execute(Server* server, Client *client, 
                IRCMessage message, std::vector<execReturnData>&execReturn);
};

#endif