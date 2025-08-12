#include "../includes/Server.hpp"
#include "../includes/Utils.hpp"
# include "../includes/commands/NICK.hpp"
# include "../includes/commands/PASS.hpp"
# include "../includes/commands/USER.hpp"
# include "../includes/commands/CAP.hpp"
# include "../includes/commands/JOIN.hpp"
# include "../includes/commands/PING.hpp"
# include "../includes/commands/QUIT.hpp"
# include "../includes/commands/PART.hpp"
# include "../includes/commands/KICK.hpp"
# include "../includes/commands/INVITE.hpp"
# include "../includes/commands/MODE.hpp"
# include "../includes/commands/TOPIC.hpp"
# include "../includes/commands/PRIVMSG.hpp"

# include "../includes/bonus/BotBonus.hpp"

bool Server::_serverIsRunning = false;
int Server::_exitStatus = 0;

Server::Server(std::string port, std::string password) : _fd(-1), _port(validatePort(port)), 
    _password(validatePassword(password)), _botManager(new Bot()) {
}


Server::~Server() {
    delete this->_botManager;
}

Bot& Server::getBotManager() {  
    if (!_botManager) {  
        throw std::runtime_error("BotManager is NULL!"); 
    }  
    return *_botManager;
}

// Getters
int const& Server::getFd(void) { return this->_fd; }

int const& Server::getPort(void) { return this->_port; }

std::string const& Server::getPassword(void) { return this->_password; }

std::vector<pollfd>& Server::getPollFds(void) { return this->_pollFds;}

bool const& Server::getIsRunning(void) { return _serverIsRunning; }

int const& Server::getExitStatus(void) { return _exitStatus; }

time_t const& Server::getServerStartTime(void) { return _serverStartTime; }

std::map<int, Client>& Server::getClients(void) {return this->_clients; }

std::map<std::string, Channel>& Server::getChannels(void) {return this->_channels; }

std::map<std::string, Command*>& Server::getCommands(void) {return this->_commands; }

Client* Server::getClientByFd(int clientFd) {
    std::map<int, Client>& clients = this->getClients();
    std::map<int, Client>::iterator it = clients.find(clientFd);
    if (it != clients.end()) {
        return &it->second;
    }
    return NULL;
}

Client* Server::getClientByNick(std::string nickname) {
    std::map<int, Client>& clients = this->getClients();
    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->second.getNick() == nickname) {
            return &it->second;
        }
    }
    return NULL;
}

Channel* Server::getChannelByName(std::string channelName) {
    std::map<std::string, Channel>& channels = this->getChannels();
    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    if (it != channels.end()) {
        return &it->second;
    }
    return NULL;
}

Command* Server::getCommandByName(std::string commandName) {
    std::map<std::string, Command*>& command = this->getCommands();
    std::map<std::string, Command*>::iterator it = command.find(commandName);
    if (it != command.end()) {
        return it->second;
    }
    return NULL;
}

pollfd Server::getServerPollFd(void) {
    pollfd serverPollFd;

    bzero(&serverPollFd, sizeof(serverPollFd));
    serverPollFd.fd = this->getFd();
    serverPollFd.events = POLLIN;
    return serverPollFd;
}

pollfd Server::getClientPollFd(int clientFd) {
    pollfd clientPollFd;
    bzero(&clientPollFd, sizeof(clientPollFd));
    clientPollFd.fd = clientFd;
    clientPollFd.events = POLLIN | POLLOUT;
    return clientPollFd;
}

// Setters
void Server::setFd(int fd) { this->_fd = fd; }

void Server::setIsRunning(bool serverIsRunning) { this->_serverIsRunning = serverIsRunning; }

void Server::setExitStatus(int exitStatus) { this->_exitStatus = exitStatus; }

void Server::setPort(int socketFd, sockaddr_in addr) {
    socklen_t addrLen = sizeof(addr);
    getsockname(socketFd, (sockaddr*)&addr, &addrLen);
    this->_port = ntohs(addr.sin_port);
    printColor("Server listening on port " + numberToString(ntohs(addr.sin_port)), ITALICGREEN);
}

void Server::setServerStartTime(time_t serverStartTime) { 
    this->_serverStartTime = serverStartTime;
}

// Check validity of the port
int Server::validatePort(std::string port) {
    if (port.empty()) {
        throw std::invalid_argument("Port is empty");
    }
    for (size_t i = 0; i < port.size(); i++) {
        if (!isdigit(port[i])) {
            throw std::invalid_argument("Port is not a number");
        }
    }
    int portNumber = atoi(port.c_str());
    if (portNumber < 0 || portNumber > 65535) {
        throw std::invalid_argument("Port is out of range");
    }
    if (portNumber >= 0 && portNumber <= 1023) {
        std::cerr << ITALICYELLOW << "Warning: Port " << portNumber << " is a well-known port. "
                << "It may conflict with system services or require admin rights." << DEF << std::endl;
    }
    return portNumber;

}

// Check validity of the password
std::string Server::validatePassword(std::string password) {
    if (password.empty()) {
        throw std::invalid_argument("Password musn't be empty.");
    }
    if (password.length() > 20) {
        throw std::invalid_argument("Password musn't exceed 20 characters.");
    }
    for (size_t i = 0; i < password.length(); i++) {
        if (isspace(password[i])) {
            throw std::invalid_argument("Password musn't contain a isspace.");
        }
    }
    return password;
}

// Check if a string contains forbidden characters
bool Server::containsForbiddenChararacters(std::string str) {
	for (size_t i = 0; i < str.length(); i++) {
		char c = str[i];
		if (c <= 0x1F || c == ' ' || c == ',' || c == ':')
			return true;
	}
	return false;
}

// Set up the server socket
void Server::setSocketOptions(int socketFd) {
    int opt = 1;
    if(setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        throw(std::runtime_error("failed to set option (SO_REUSEADDR) on socket"));
    }
    if (setsockopt(socketFd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) == -1) {
        throw std::runtime_error("Failed to set option (SO_KEEPALIVE) on socket");
    }
}

// Bind and listen on the server socket
void Server::setServerSocket(int socketFd) {
    this->setSocketOptions(socketFd);
    
    int port = this->getPort();

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (fcntl(socketFd, F_SETFL, O_NONBLOCK) < 0) {
        close(socketFd);
        throw std::runtime_error("Failed to set socket to non-blocking mode");
    }
    if (bind(socketFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        close(socketFd);
        throw std::runtime_error("Failed to bind socket");
    }
    if (this->getPort() == 0) {
        setPort(socketFd, serverAddr);
    }
    if (listen(socketFd, SOMAXCONN) < 0) {
        close(socketFd);
        throw std::runtime_error("Failed to listen on socket");
    }
}

// Signal handler for server shutdown
void Server::signalHandler(int status) {
    _serverIsRunning = false; 
    _exitStatus = 128 + status; 
    std::cout << "\n";
    printColor("[SIGNAL RECEIVED]:  Server shutting down...", BOLDRED);
}

// Setup signal handlers (CTRL+C, SIGTERM, SIGQUIT)
void Server::setupSignalHandlers(void) {
    struct sigaction action;

    bzero(&action, sizeof(action)); 
    action.sa_handler = Server::signalHandler; 
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0; 
    sigaction(SIGINT, &action, NULL); 
    sigaction(SIGTERM, &action, NULL); 
    sigaction(SIGQUIT, &action, NULL); 
    signal(SIGTSTP, SIG_IGN); 
}

// Setup all commands
void Server::setupCommands(void) {
    std::map<std::string, Command*>& commands = this->getCommands();

    commands.insert(std::make_pair("NICK", new NICK));
    commands.insert(std::make_pair("PASS", new PASS));
    commands.insert(std::make_pair("CAP", new CAP()));
    commands.insert(std::make_pair("USER", new USER()));
    commands.insert(std::make_pair("PING", new PING()));  
    commands.insert(std::make_pair("QUIT", new QUIT()));
    commands.insert(std::make_pair("JOIN", new JOIN()));
    commands.insert(std::make_pair("PRIVMSG", new PRIVMSG()));
    commands.insert(std::make_pair("INVITE", new INVITE()));
    commands.insert(std::make_pair("KICK", new KICK()));
    commands.insert(std::make_pair("PART", new PART()));
    commands.insert(std::make_pair("MODE", new MODE()));
    commands.insert(std::make_pair("TOPIC", new TOPIC()));
}

// Check validity of nickname
bool Server::isValidNickname(std::string nickname) {
    if (nickname.empty() || nickname.length() > 9) {
        return false;
    }
    const std::string specialChars = "[]{}\\|_-.";
    const std::string forbiddenFirstChar = "#:";

    char firstChar = nickname[0];
    if (forbiddenFirstChar.find(firstChar) != std::string::npos ||
        (!std::isalpha(firstChar) && specialChars.find(firstChar) == std::string::npos)) {
        return false;
    }
    for (size_t i = 1; i < nickname.length(); i++) {
        char c = nickname[i];
        if (!std::isalnum(c) && specialChars.find(c) == std::string::npos) {
            return false;
        }
    }
    return true;
}

// Check if nickname is already used
bool Server::isNickNameAlreadyUsed(std::string nickname) {
    Client *client = this->getClientByNick(nickname);
    if (client != NULL) {
        return true;
    }
    return false;
}

// Create basic exec return data
execReturnData Server::createBasicExecReturnData(int fd) {
    execReturnData data;
    data.clientsFds.push_back(fd);
    return data;
}

// Remove a channel
void Server::rmChannel(std::string channelName) {
    std::map<std::string, Channel>& channel = this->getChannels();
    std::map<std::string, Channel>::iterator it = channel.find(channelName);
    if (it != channel.end()) {
        std::set<int> joinedClientsFdsSet = it->second.getJoinedClientsFds();
        std::vector<int> joinedClientsFds(joinedClientsFdsSet.begin(), joinedClientsFdsSet.end());
        for (std::vector<int>::iterator it2 = joinedClientsFds.begin(); it2 != joinedClientsFds.end(); ++it2) {
            Client* client = this->getClientByFd(*it2);
            if (client) {
                client->partChannel(channelName);
            }
        }
        channel.erase(it);
    } 
}

// Check validity of channel name and topic
bool Server::isValidChannelName(std::string channelName) {
    if (channelName.empty() || channelName.length() > 50 
        || channelName[0] != '#' || channelName.length() == 1) {
        return false;
    }

    for (size_t i = 1; i < channelName.length(); ++i) {
        char c = channelName[i];
        if (c == ' ' || c == ',' || c == ':' || c == '\0' || static_cast<unsigned char>(c) < 32) {
            return false;
        }
    }
    return true;
}

// Check validity of channel topic
bool Server::isValidChannelTopic(std::string topic) {
    if (topic.empty() || topic.length() > MAX_TOPIC_LENGTH) {
        return false;
    }
    for (size_t i = 0; i < topic.length(); i++) {
        char c = topic[i];
        if (c < 32 || c > 126) { 
            return false;
        }
    }
    return true;
}

// Check if a string is a positive number
bool Server::isStringPositiveNumber(std::string str) {
    if (str.empty()) {
        return false;
    }
    for (size_t i = -1; i < str.length(); i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

// Add a new client
void Server::addClient(int clientFd) {
    std::map<int, Client>& clients = this->getClients();
	clients.insert(std::make_pair(clientFd, Client(clientFd, this)));
	printSuccess("A new client with FD n°" + numberToString(clientFd) + " just arrived! Welcome to you! ^^");
}

// Remove a client
void Server::rmClient(int clientFd) {
	std::map<int, Client>& clients = this->getClients();

	std::map<int, Client>::iterator clientIt = clients.find(clientFd);
	if (clientIt == clients.end())
		return;

	Client& client = clientIt->second;
	std::vector<std::string> joinedChannels = client.getJoinedChannelsNames();

	for (std::vector<std::string>::iterator chanIt = joinedChannels.begin(); chanIt != joinedChannels.end(); ++chanIt) {
		Channel* channel = this->getChannelByName(*chanIt);
		if (channel) {
			channel->uninvite(clientFd);
			channel->ungrantOperator(clientFd);
			channel->part(clientFd);
		}
	}
	close(client.getFd());
	clients.erase(clientIt);
}

// Handle client disconnection
void Server::handleClientDisconnection(Client &client, int pollFd, std::vector<pollfd> &serverPollFds, int i) {
    if (client.hasUsedQuitCommand()) {
        printSuccess("Client on fd n°" + numberToString(pollFd) + " used the QUIT command. Disconnecting...");
        this->rmClient(pollFd);
        serverPollFds.erase(serverPollFds.begin() + i--);
    } else if (!client.isLoggedIn() && (std::time(NULL) - client.getConnectionStartTime() >= AUTO_LOGOUT_TIMEOUT)) {
        printSuccess("Client on fd n°" + numberToString(pollFd) 
            + " did not authenticate within " + numberToString(AUTO_LOGOUT_TIMEOUT)
            + "s. Auto disconnecting...");
        this->rmClient(pollFd);
        serverPollFds.erase(serverPollFds.begin() + i--);
    }
}

// Launcher the server
void Server::launch(void) {
    this->setFd(socket(AF_INET, SOCK_STREAM, 0));
    int fd = this->getFd();
    if (fd < 0) {
        throw std::runtime_error("Failed to create socket");
    }
    this->setServerSocket(fd);
    this->setServerStartTime(std::time(NULL));
    this->setIsRunning(true);
    this->setupCommands();
    this->setupSignalHandlers();
    this->_botManager->setupBot(this);

    printColor("[SERVER STARTED]:  • Port: " + numberToString(this->getPort()) + "\n" 
            + "\t\t\t\t• " + "Password: " + this->getPassword(), BOLDGREEN);
    printColor("[SERVER READY]:  Press CTRL + C to stop the server.", BOLDYELLOW);

    std::vector<pollfd>& serverPollFds = this->getPollFds();
    int botFd = this->_botManager->getBotFd();
    serverPollFds.push_back(this->getServerPollFd());
    serverPollFds.push_back(this->getClientPollFd(botFd));

    while (this->getIsRunning()) {
        if (poll(serverPollFds.data(), serverPollFds.size(), -1) < 0) {
            if (!this->getExitStatus()) {
                this->setExitStatus(1);
                this->setIsRunning(false);
                printColor("[SERVER ERROR]:  Poll error", BOLDRED);
            }
            break;
        }

        for (size_t i = 0; i < serverPollFds.size(); i++) {
            pollfd* it = &serverPollFds[i];
            
            int pollFd = it->fd;
            if (it->revents & POLLERR) {
                int error = 1;
                socklen_t length = sizeof(error);

                serverPollFds.erase(serverPollFds.begin() + i--);
                if (!getsockopt(pollFd, SOL_SOCKET, SO_ERROR, &error, &length)) {
                    printWarning("Socket error on fd: " + numberToString(pollFd) 
                            + ": " + strerror(error));
                }
            } else if (it->revents & POLLOUT && pollFd == botFd) {
                this->_botManager->handleBotPollOut(pollFd, this);
            } else if (it->revents & POLLIN && pollFd == fd) {
                int clientFd = accept(pollFd, NULL, NULL);
                if (clientFd < 0) {
                    printWarning("Cannot accept new client on fd: " + numberToString(pollFd));
                    continue;
                } else if (clientFd > FD_SETSIZE) {
                    printWarning("Cannot accept new client on fd: " + numberToString(clientFd) 
                                + ", exceeds max fd: " + numberToString(FD_SETSIZE));
                    close(clientFd);
                    continue;
                }
                this->setSocketOptions(clientFd);
                serverPollFds.push_back(this->getClientPollFd(clientFd));
                this->addClient(clientFd);
            } else if (it->revents & POLLIN || it->revents & POLLOUT) {
                Client* currentClient = this->getClientByFd(pollFd);
                if (!currentClient) {
                    printWarning("Cannot find client on fd: " + numberToString(pollFd));
                    continue;
                }
                
                if (it->revents & POLLIN) {
                    char buffer[MESSAGE_SIZE];
                    bzero(buffer, sizeof(buffer));
                    ssize_t bytesRead = recv(pollFd, buffer, sizeof(buffer) - 1, 0);
                    if (bytesRead <= 0) {
                        this->rmClient(pollFd);
                        serverPollFds.erase(serverPollFds.begin() + i--);
                        printWarning("Client on fd: " + numberToString(pollFd) + " disconnected from the server. Bye!");
                    } else {
                        std::string& inData = currentClient->getIndata();
                        std::string strBuffer(buffer);

                        size_t newlinePos = strBuffer.find("\n");
                        size_t carriageReturnPos  = strBuffer.find("\r");
                        
                        size_t length = strBuffer.length() 
                                            - (newlinePos != std::string::npos)
                                            - (carriageReturnPos != std::string::npos);
                        if (newlinePos != std::string::npos && newlinePos == length - 1
                            && (carriageReturnPos == std::string::npos || carriageReturnPos != length - 2)) {
                            strBuffer = strBuffer.substr(0, newlinePos) + CRLF;
                        }
                        inData += strBuffer;
                        if (length > 0) {
                            printData("Client on fd(" + numberToString(pollFd) 
                                    + ") send data(" + numberToString(length) + " bytes)");
                        }
                        currentClient->parseInData();
                    }
                } else {
                    Client* bot = this->_botManager->getBot();
					std::string& outDataBot = currentClient->getOutdata(); // OK
					if (bot && currentClient->getFd() == bot->getFd()) {
                        outDataBot.clear();
						continue;
					}

                    std::string& outData = currentClient->getOutdata();
                    size_t pos = outData.find(CRLF);
                    if(pos != std::string::npos) {
                        std::string dataToSend = outData.substr(0, pos + 2);
                        send(pollFd, dataToSend.c_str(), dataToSend.length(), MSG_NOSIGNAL);
                        outData.erase(0, pos + 2);
                    }
                    handleClientDisconnection(*currentClient, pollFd, serverPollFds, i);
                }
            }
        }
    }
}

// Cleanup the server
void Server::cleanup(void) {
    std::vector<pollfd>& pollFds = this->getPollFds();
    for (std::vector<pollfd>::iterator it = pollFds.begin(); it != pollFds.end(); ++it) {
        close(it->fd);
    }
    pollFds.clear();

    std::map<std::string, Command*>& commands = this->getCommands();
    for (std::map<std::string, Command*>::iterator it = commands.begin(); 
        it != commands.end(); ++it) {
        if (it->second) {
			delete it->second;
			it->second = NULL;
		}
    }
    commands.clear();
    
    close(this->getFd());
    printColor("[SERVER STOPPED]:  Server stopped.", BOLDRED);
}