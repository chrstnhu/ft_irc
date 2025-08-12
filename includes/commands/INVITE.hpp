#ifndef INVITE_HPP
#define INVITE_HPP

#include "../Command.hpp"

class INVITE : public Command {
    public :
        INVITE(void);
        virtual ~INVITE(void);

        // Execute the command
        virtual void execute(Server* server, Client *client, 
                IRCMessage message, std::vector<execReturnData>&execReturn);
};

#endif