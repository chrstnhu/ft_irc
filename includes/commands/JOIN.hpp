#ifndef JOIN_HPP
#define JOIN_HPP

#include "../Command.hpp"

class JOIN : public Command {
public:
    JOIN(void);
    virtual ~JOIN(void);

    virtual void execute(Server* server, Client* client, 
            IRCMessage message, std::vector<execReturnData>& execReturn);
};

#endif
