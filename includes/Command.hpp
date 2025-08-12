#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Includes.hpp"
#include "Server.hpp"
#include "Server.hpp"
#include "Client.hpp"

struct IRCMessage;
struct execReturnData;
class Server;
class Client;

class Command {
    private :
        std::string _name;
        int         _requiredParams;
        bool        _mustBeLoggedIn;

    public :
        Command(std::string name, int requiredParams, bool mustBeLoggedIn);
        virtual ~Command();

        // Getters
        std::string const &getName(void);
        int const &getRequiredParamsNumber(void);
        bool const &getMustBeLoggedIn(void);

        // Execute the command
        virtual void execute(Server* server, Client *client, 
                IRCMessage message, std::vector<execReturnData>&execReturn) = 0;
};

#endif 