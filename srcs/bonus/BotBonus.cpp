#include "../../includes/bonus/BotBonus.hpp"

Bot::Bot() : _botFd(0), _sentBasicBotData(false), _bot(NULL)  { }

Bot::~Bot() { }

int const& Bot::getBotFd(void) { return this->_botFd; }

bool const& Bot::hasSentBasicBotData(void) { return this->_sentBasicBotData; }

Client* Bot::getBot(void) { return this->_bot; }

void Bot::setBotFd(int botFd) { this->_botFd = botFd; }

void Bot::setSentBasicBotData(bool sentBasicBotData) { this->_sentBasicBotData = sentBasicBotData; }

void Bot::setBot(Client* bot) { this->_bot = bot; }

// Sets up the bot client by creating a socket and connecting to the server
void Bot::setupBot(Server* server) {
    int botFd = socket(AF_INET, SOCK_STREAM, 0); 

    if (botFd < 0) {
        throw std::runtime_error("Cannot create the BOT client.");
	}
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server->getPort());
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(botFd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        close(botFd);
        throw std::runtime_error("Cannot connect the BOT Client to the server.");
    }
    this->setBotFd(botFd);
}

// Runs the bot command based on the message received from the client
void Bot::runBotCommand(Client* client, Channel* channel, std::string message) {
	Client* bot = this->getBot();
	std::string& inData = bot->getIndata();

	std::vector<std::string> params = split(message, ' ');
	std::string command = params[0];
	char flag = BOT_NONE;
	params.erase(params.begin());

	if (command[0] == BOT_PREFIX) {
		command = command.substr(1);
		std::string output = "Cannot find " BOLDDEF + command 
			+ DEF " bot command, check the bot commands list by using the !help [command].";
		if (!channel->hasJoined(bot->getFd())) {
			if (command == "bot") {
				output = RPL_JOIN(bot->getFullUserId(), channel->getName() + " " + channel->getPassword()) + CRLF;
				flag = 'J';
			} else {
		        output = "I'm already here, no need to invite me twice! 😄";
    		}
		} else {
			if (command == "bot") {
				output = "I'm already here, no need to invite me twice! 😄";
			} else if (command == "rpn") {
				std::string rpn;
				for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++) {
					rpn += *it + " ";
				}
				if (rpn.empty()) {
					output = "[RPN] Usage: !rpn <RPN formula>, please provide a valid RPN formula.";
				}
				try {
					output = "[RPN] The result of your RPN formula is: " + numberToString(RPN::evalRPN(rpn)) + " !";
				} catch (std::exception const& e) {
					output = "[RPN] Error: " + std::string(e.what());
				}
			} else if (command == "dice") {
				output = diceGame(); 
			} else if (command == "flip") {
				output = flipGame();
			} else if (command == "rps") {
				output = rpsGame(params);
			} else if (command == "slot") {
				output = slotGame();
			} else if (command == "troll") {
				output = trollGame();
			} else if (command == "whoami") {
				output = handleWhoami(client, channel);
			} else if (command == "part") {
				output = handlePartCommand(client, channel, params);
				flag = BOT_PART;
			} else if (command == "help") {
				output = getHelpMessage(params);
			}
		}

		if (flag == BOT_NONE) {
			output = RPL_PRIVMSG(bot->getFullUserId(), "#" + channel->getName(), client->getNick() + ": " + output) + CRLF;
		}

		if (flag == BOT_JOIN || channel->hasJoined(bot->getFd())) {
			if (flag == BOT_PART) {
				std::string partMsg = "Bye bye everyone! (Removed by " + client->getNick() + ")";
				inData += RPL_PRIVMSG(bot->getFullUserId(), "#" + channel->getName(), partMsg) + CRLF;
			}
			inData += output;
			if (flag == BOT_JOIN) {
				std::string startMsg = ": Hello! I'm your friendly bot 🤖 — type !help to see what I can do!";
				inData += RPL_PRIVMSG(bot->getFullUserId(), "#" + channel->getName(), client->getNick() + startMsg) + CRLF;
			}
		}
	}
}

// Handles the bot poll out by sending the bot's data to the server
void Bot::handleBotPollOut(int pollFd, Server* server) {
    if (!this->hasSentBasicBotData()) {
        std::string logString;
        logString += "PASS " + server->getPassword() + CRLF;
        logString += "NICK " BOT_NICKNAME CRLF;
        logString += "USER " BOT_USER CRLF;
        send(pollFd, logString.c_str(), logString.size(), MSG_NOSIGNAL);
        this->setSentBasicBotData(true);
    } else {
        Client* bot = this->getBot();
        if (bot) {
            std::string& inData = bot->getIndata();
            size_t pos = inData.find(CRLF);
            if (pos != std::string::npos) {
                std::string message = inData.substr(0, pos + 2);
                send(pollFd, message.c_str(), message.length(), MSG_NOSIGNAL);
                inData.erase(0, pos + 2);
            }
        }
    }
}
