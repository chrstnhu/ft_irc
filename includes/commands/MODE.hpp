#ifndef MODE_HPP
#define MODE_HPP

#include "../Command.hpp"

class MODE : public Command {
    public:
        MODE(void);
        virtual ~MODE(void);

        virtual void execute(Server* server, Client* client, 
                IRCMessage message, std::vector<execReturnData>& execReturn);
};

#endif