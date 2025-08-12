#ifndef USER_hpp
#define USER_hpp

#include "../Command.hpp"

class USER : public Command {
    public :
        USER(void);
        virtual ~USER(void);

        // Execute the command
        virtual void execute(Server* server, Client *client, 
                IRCMessage message, std::vector<execReturnData>&execReturn);
};

#endif