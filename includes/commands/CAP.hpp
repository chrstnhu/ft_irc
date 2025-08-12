#ifndef CAP_HPP
#define CAP_HPP

#include "../Command.hpp"

class CAP : public Command {
    private :
        std::vector<std::string> _capabilitiesList;
    
    public :
        CAP(void);
        virtual ~CAP(void);

        std::string vecToStr(std::vector<std::string> vec);

        virtual void execute(Server* server, Client *client, 
                IRCMessage message, std::vector<execReturnData>&execReturn);
};

#endif