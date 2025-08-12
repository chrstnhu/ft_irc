#include "../includes/Client.hpp"

Client::Client(int const fd, Server *server):_fd(fd), _nickname(""), _username(""), _realname(""), _hostname(""), _servername("ft_irc"),_server(server), _connectionStartTime(std::time(NULL)), _isPasswordNeeded(true), _hasUsedQuitCommand(false) {}

Client::~Client() {}

// Getters
int const &Client::getFd() { return _fd; }

int const &Client::getFd() const { return _fd; }

std::string const &Client::getNick() { return _nickname; }

std::string const &Client::getUsername() { return _username; }

std::string const &Client::getRealname() { return _realname; }

std::string const &Client::getHostname() { return _hostname; }

std::string const &Client::getServername() { return _servername; }

time_t const &Client::getConnectionStartTime() { return _connectionStartTime; }

std::string  &Client::getIndata() { return _inData; }

std::string  &Client::getOutdata() { return _outData; }

std::vector<std::string> const &Client::getJoinedChannelsNames() { return _joinedChannelsNames; }

Server *Client::getServer() { return _server; }

/*
* Get the file descriptors of clients who have joined the channels that this client is part of.
* It returns a vector of unique file descriptors, excluding the client's own file descriptor.
*/
std::vector<int> Client::getJoinedChannelsFds() {
    std::set<int> uniqueFds;

    for (std::vector<std::string>::const_iterator it = _joinedChannelsNames.begin(); it != _joinedChannelsNames.end(); ++it) {
        Channel* channel = _server->getChannelByName(*it);
        if (channel) {
            const std::set<int>& clientFds = channel->getJoinedClientsFds(); 
            for (std::set<int>::const_iterator fdIt = clientFds.begin(); fdIt != clientFds.end(); ++fdIt) {
                if (*fdIt != this->_fd) {
                    uniqueFds.insert(*fdIt);
                }
            }
        } else {
            printWarning("Channel " + *it + " not found for FD " + numberToString(_fd));
        }
    }

    return std::vector<int>(uniqueFds.begin(), uniqueFds.end());
}


/*
* Get the full user ID in the format: :nickname!username@hostname
* If the hostname is empty, it defaults to "localhost".
*/
std::string Client::getFullUserId() {
    std::string hostname = _hostname.empty() ? "localhost" : _hostname;
    return ":" + _nickname + "!" + _username + "@" + hostname;
}

// Setters
void Client::setNick(std::string nick) { _nickname = nick; }

void Client::setUsername(std::string username) { _username = username; }

void Client::setHostname(std::string hostname) { _hostname = hostname; }

void Client::setRealname(std::string realname) { _realname = realname; }

void Client::setServername(std::string servername) { _servername = servername; }

void Client::setIsPasswordNeeded(bool isneeded) { _isPasswordNeeded = isneeded; }

void Client::setHasusedQuitCommand(bool hasused) { _hasUsedQuitCommand = hasused; }


// Check if a password is needed for the client.
bool Client::isPasswordNeeded() { return _isPasswordNeeded; }

// Check if the client has used the QUIT command.
bool Client::hasUsedQuitCommand() { return _hasUsedQuitCommand; }

// Check if the client is logged in.
bool Client::isLoggedIn()
{
    if (this->isPasswordNeeded())
        return false;
    if (this->getNick() == "") 
        return false;
    if (this->getUsername() == "") 
        return false;
    if (this->getRealname() == "") 
        return false;
    return true;
}


/*
* Add a channel name to joinedChannelsNames.
* If the channel name is already present, it does nothing.
*/
void Client::joinChannel(std::string name) {
    if (std::find(_joinedChannelsNames.begin(), _joinedChannelsNames.end(), name) == _joinedChannelsNames.end()){
        _joinedChannelsNames.push_back(name);
    }
}

/*
* Remove a channel name from joinedChannelsNames.
* If the channel name is not found, it does nothing.
*/
void Client::partChannel(std::string name) {
    _joinedChannelsNames.erase(
        std::remove(_joinedChannelsNames.begin(), _joinedChannelsNames.end(), name),
        _joinedChannelsNames.end()
    );
}

/*
* Extract the next line from the input data.
* The line is terminated by CRLF (\r\n), LF (\n), or CR (\r).
*/
bool Client::extractNextLine(std::string& inData, std::string& rawLine)  {
    size_t pos = inData.find(CRLF);
    if (pos == std::string::npos) {
        pos = inData.find('\n');
        if (pos == std::string::npos) {
            pos = inData.find('\r');
        }
    }
    if (pos == std::string::npos) {
        return false;
    }
    
    rawLine = inData.substr(0, pos);
    inData.erase(0, pos + (inData[pos] == '\r' && pos + 1 < inData.size() && inData[pos + 1] == '\n' ? 2 : 1));
    return true;
}

/*
* parse a raw IRC message line into an IRCMessage struct, 
* format of an IRC message is: <prefix>(optional) <command> <params> :<text>(optional)
* example: :nick!user@host PRIVMSG #channel :Hello, world!   prefix is the sender's nickname, user, and host. here is nick!user@host
*/
IRCMessage Client::parseIRCMessage(const std::string& rawLine) {
    IRCMessage message;
    message.hasText = false;

    if (rawLine.empty()) {
        return message;
    }

    std::istringstream stream(rawLine);
    std::string token;

    if (rawLine[0] == ':') {
        if (!std::getline(stream, token, ' ') || token.length() <= 1) {
            return message;
        }
        message.prefix = token.substr(1);
    }

    if (std::getline(stream, token, ' ')) {
        for (size_t i = 0; i < token.size(); ++i) {
            token[i] = std::toupper(token[i]);
        }
        message.command = token;
    }

    while (std::getline(stream, token, ' ')) {
        if (token.empty()) {
            continue;
        }
        if (token[0] == ':') {
            message.hasText = true;
            message.text = token.substr(1);
            std::string rest;
            while (std::getline(stream, rest, '\0')) {
                if (!rest.empty()) {
                    message.text += " " + rest;
                }
            }
            break;
        }
        message.params.push_back(token);
    }
    return message;
}

/**
 * distribute response message to specified clients based on their file descriptors.
 * store the message in the outData attribute of each client.
*/
void Client::distributeResponses(Server* server, const std::vector<execReturnData>& responses) {
    for (std::vector<execReturnData>::const_iterator it = responses.begin(); it != responses.end(); ++it) {
        if (it->message.empty()) {
            continue;
        }
        for (std::vector<int>::const_iterator fdIt = it->clientsFds.begin(); fdIt != it->clientsFds.end(); ++fdIt) {
            Client* client = server->getClientByFd(*fdIt);
            if (client) {
                std::string& outData = client->getOutdata();
                outData += it->message;
                if (outData.substr(outData.size() - 2) != CRLF) {
                    outData += CRLF; 
                }
            }
        }
    }
}

/*
 * Parse incoming data from the client, extract IRC messages, and execute commands.
*/
void Client::parseInData(void) {
    Server* server = this->getServer();
    std::string& inData = this->getIndata();
    std::string rawLine;

    const size_t MAX_BUFFER_SIZE = 8192;
    if (inData.size() > MAX_BUFFER_SIZE) {
        printWarning("Input buffer too large for client FD n°" + numberToString(this->getFd()));
        inData.clear();
        return;
    }

    while (extractNextLine(inData, rawLine)) {
        try {
            if (rawLine.empty()) {
                continue;
            }
            IRCMessage parsedMessage = parseIRCMessage(rawLine);
            std::vector<execReturnData> responses = execParsedMessage(parsedMessage);
            distributeResponses(server, responses);
        }
		catch (const std::exception& err) {
            printWarning("Error parsing data for client FD n°" + numberToString(this->getFd()) + ": " + err.what());
            std::string errorMsg = ERR_UNKNOWNERROR(this->getNick(), "Parse error");
            std::vector<execReturnData> errorResponse;
            errorResponse.push_back(server->createBasicExecReturnData(this->getFd()));
            errorResponse.back().message = errorMsg + CRLF;
            distributeResponses(server, errorResponse);
        }
    }
}

/**
 * validate the command and its parameters and handle several error cases: empty command, unknown command, insufficient parameters, and not logged in.
 * if errors are found, return an error message in the execReturnData structure errorData.
 * if no errors are found, return an empty message in the execReturnData structure successData with the client fd in the clientsFds vector.
*/
execReturnData Client::validateCommand(const IRCMessage& parsedMessage) {
    Server* server = this->getServer();
    execReturnData errorData = server->createBasicExecReturnData(this->getFd());
    std::string command = parsedMessage.command;	
    std::vector<std::string> params = parsedMessage.params;

    if (command.empty()) {
        errorData.message = ERR_NEEDMOREPARAMS(this->getNick(), "*");
        printWarning("Cannot run an empty command.");
        return errorData;
    }

    Command* selectedCommand = server->getCommandByName(command);
    if (!selectedCommand) {
        errorData.message = ERR_UNKNOWNCOMMAND(this->getNick(), command);
        printWarning("Cannot find " + command + " command.");
        return errorData;
    }

    if (static_cast<int>(params.size()) < selectedCommand->getRequiredParamsNumber()) {
        errorData.message = ERR_NEEDMOREPARAMS(this->getNick(), command);
        printWarning(command + " command needs at least " 
                    + numberToString(selectedCommand->getRequiredParamsNumber()) 
                    + " params (given " + numberToString(params.size()) + ").");
        return errorData;
    }

    if (!this->isLoggedIn() && selectedCommand->getMustBeLoggedIn()) {
        errorData.message = ERR_NOTREGISTERED(this->getNick());
        printWarning(command + " needs to be run by a logged in client.");
        return errorData;
    }

    execReturnData successData;
    successData.clientsFds.push_back(this->getFd());
    successData.message = "";
    return successData;
}

/*
* Generate welcome messages for the client when they log in.
*/
void Client::generateWelcomeMessages(std::vector<execReturnData>& returnData) {
    Server* server = this->getServer(); 
    execReturnData welcomeData = server->createBasicExecReturnData(this->getFd());
    
    std::ostringstream oss; 
    oss << RPL_WELCOME(this->getFullUserId(), this->getNick(), this->getServername()) << CRLF
        << RPL_YOURHOST(this->getNick(), this->getServername()) << CRLF
        << RPL_CREATED(this->getNick(), getFormattedTime(server->getServerStartTime())) << CRLF 
        << RPL_ISUPPORT(this->getNick()) << CRLF; 
    
    welcomeData.message = oss.str();
    returnData.push_back(welcomeData);
}

/*
* Execute the parsed IRC message command.
* Validate the command, execute it, and generate welcome messages if the client has just logged in.
*/
std::vector<execReturnData> Client::execParsedMessage(IRCMessage parsedMessage) {
    std::vector<execReturnData> returnData;

    execReturnData validateData = validateCommand(parsedMessage);
    if (!validateData.message.empty()) {
        returnData.push_back(validateData);
        return returnData;
    }

    Server* server = this->getServer();
    Command* selectedCommand = server->getCommandByName(parsedMessage.command);
    bool isLoggedInBeforeExecution = this->isLoggedIn();

    printInfo("Client FD n°" + numberToString(this->getFd()) + " executing command: " + parsedMessage.command);
    try {
        selectedCommand->execute(server, this, parsedMessage, returnData);
    }
	catch (const std::exception& err) {
        execReturnData errorData = server->createBasicExecReturnData(this->getFd());
        errorData.message = ERR_UNKNOWNERROR(this->getNick(), "Command execution failed: " + std::string(err.what())) + CRLF;
        returnData.push_back(errorData);
        printWarning("Command " + parsedMessage.command + " failed for client FD n°" + numberToString(this->getFd()) + ": " + err.what());
    }

    if (!isLoggedInBeforeExecution && this->isLoggedIn()) {
        generateWelcomeMessages(returnData);
    }

    return returnData;
}
