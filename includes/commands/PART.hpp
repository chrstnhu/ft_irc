#ifndef PART_HPP
#define PART_HPP

#include "../Command.hpp"

class PART : public Command {
    public :
        PART(void);
        virtual ~PART(void);

        virtual void execute(Server* server, Client *client, 
                IRCMessage message, std::vector<execReturnData>&execReturn);
};

#endif