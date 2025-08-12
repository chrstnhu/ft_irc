
#ifndef TOPIC_HPP
# define TOPIC_HPP

# include "../Command.hpp"

class TOPIC : public Command {
    public:
        TOPIC(void);
        virtual ~TOPIC(void);
        
        virtual void execute(Server* server, Client* client, 
                IRCMessage message, std::vector<execReturnData>& execReturn);
};

#endif