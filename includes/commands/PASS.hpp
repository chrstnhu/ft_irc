#ifndef PASS_HPP
#define PASS_HPP

#include "../Command.hpp"

class PASS : public Command {
    public:
        PASS();
        virtual ~PASS();

        virtual void execute(Server* server, Client* client, 
                IRCMessage message, std::vector<execReturnData>& execReturn);

};

#endif