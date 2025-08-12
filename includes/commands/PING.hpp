#ifndef PING_HPP
#define PING_HPP

#include "../Command.hpp"

class PING : public Command {
	public:
		PING();
		virtual ~PING();

		virtual void execute(Server* server, Client* client, 
				IRCMessage message, std::vector<execReturnData>& execReturn);
};




#endif 
