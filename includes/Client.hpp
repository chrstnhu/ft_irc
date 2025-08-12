#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "Command.hpp"

class Server;
class Channel;
class Command;

struct execReturnData;
struct IRCMessage;

class Client
{
private:
    int _fd;
    std::string _nickname;
    std::string _username;
    std::string _realname;
    std::string _hostname;
	std::string _servername; 

    Server *_server;

    time_t _connectionStartTime;
    bool _isPasswordNeeded;
    bool _hasUsedQuitCommand;

    std::string                 _inData;
    std::string                 _outData;
    std::vector<std::string> _joinedChannelsNames;



public:
    
    //constructor,destructor,getters,setters
    Client(int const fd, Server *server);
    ~Client();
    int const &getFd();
    int const &getFd() const;

    std::string const &getNick();
    std::string const &getUsername();
    std::string const &getRealname();
    std::string const &getHostname();
	std::string const &getServername();
    time_t const &getConnectionStartTime();
    bool isPasswordNeeded();
    bool hasUsedQuitCommand();
    std::string  &getIndata();
    std::string  &getOutdata();
    std::vector<std::string> const &getJoinedChannelsNames();
    Server *getServer();
    bool isLoggedIn();
    std::string getFullUserId();
    std::vector<int> getJoinedChannelsFds();

    void setNick(std::string nick);
    void setUsername(std::string username);
    void setRealname(std::string realname);
    void setHostname(std::string hostname);
	void setServername(std::string servername);
    void setIsPasswordNeeded(bool isneeded);
    void setHasusedQuitCommand(bool hasused);
    void joinChannel(std::string name);
    void partChannel(std::string name);

    bool extractNextLine(std::string& inData, std::string& rawLine);
	IRCMessage parseIRCMessage(const std::string& rawLine);
	void distributeResponses(Server* server, const std::vector<execReturnData>& responses);
	void parseInData();

	execReturnData validateCommand(const IRCMessage& parsedMessage);
	void generateWelcomeMessages(std::vector<execReturnData>& returnData);
    std::vector<execReturnData> execParsedMessage(IRCMessage parsedMessage);
};



#endif 