#ifndef QUIT_HPP
#define QUIT_HPP

#include "../Command.hpp"

class QUIT : public Command {
    public :
        QUIT(void);
        virtual ~QUIT(void);

        virtual void execute(Server* server, Client *client, 
                IRCMessage message, std::vector<execReturnData>&execReturn);
};

#endif