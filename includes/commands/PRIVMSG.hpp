#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "../Command.hpp"

class PRIVMSG : public Command {
public:
	PRIVMSG(void);
	virtual ~PRIVMSG(void);

	virtual void execute(Server* server, Client* client, 
			IRCMessage message, std::vector<execReturnData>& execReturn);
};

#endif 