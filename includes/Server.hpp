#ifndef SERVER_HPP
# define SERVER_HPP

# include <poll.h>
# include "Includes.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "Command.hpp"
# include "bonus/BotBonus.hpp"

class Client;
class Channel;
class Command;
class Bot;

struct execReturnData;

class Server {
    private:
        int             _fd;
        int             _port;
        std::string     _password;
        time_t          _serverStartTime;

        std::map<int, Client>           _clients;
        std::map<std::string, Channel>  _channels;
        std::map<std::string, Command*> _commands;
        
        std::vector<pollfd> _pollFds;
        
        static bool _serverIsRunning;
        static int  _exitStatus;

        Bot*    _botManager;

    public:
        Server(std::string port, std::string password);
        ~Server();

        // Getters
        int const& getFd(void);
		int const& getPort(void);
		std::string const& getPassword(void);
        std::vector<pollfd>& getPollFds(void);
        bool const& getIsRunning(void);
        int const& getExitStatus(void);
        time_t const& getServerStartTime(void);

        std::map<int, Client>& getClients(void);
        std::map<std::string, Channel>& getChannels(void);
        std::map<std::string, Command*>& getCommands(void);

        Client* getClientByFd(int clientFd);
        Client* getClientByNick(std::string nickname);
        Channel* getChannelByName(std::string channelName);
        Command* getCommandByName(std::string commandName);

        pollfd getServerPollFd(void);
        pollfd getClientPollFd(int clientFd);   
     
        // Setters
        void setFd(int fd);
        void setExitStatus(int exitStatus);
        void setServerStartTime(time_t serverStartTime);
        void setPort(int socketFd, sockaddr_in addr);

        // Check the validity of the port and password
        int validatePort(std::string port);
        std::string validatePassword(std::string password);
        bool containsForbiddenChararacters(std::string str);

        // Launch the server
        void launch(void);
		void setIsRunning(bool isRunning);
        void cleanup(void);
        static Server* instance;

        // Server socket
        void setServerSocket(int socketFd);
        void setSocketOptions(int socketFd);
    
        // Signal
        void setupSignalHandlers(void);
        static void signalHandler(int status);

        // Client
        void addClient(int clientFd);
        void rmClient(int clientFd);
        void handleClientDisconnection(Client &client, int pollFd, std::vector<pollfd> &serverPollFds, int i);

        // Command
        void setupCommands(void);
        bool isValidNickname(std::string nickname);
        bool isNickNameAlreadyUsed(std::string nickname);
        static execReturnData createBasicExecReturnData(int fd);

        // Channel
        void rmChannel(std::string channelName);
        bool isValidChannelName(std::string channelName);
        bool isValidChannelTopic(std::string topic);
        bool isStringPositiveNumber(std::string str);

        // Bonus BOT
        Bot& getBotManager();
};


# endif