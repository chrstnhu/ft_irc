#ifndef BOTBONUS_HPP
#define BOTBONUS_HPP

#include "../Includes.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"
#include "RPN.hpp"

class Server;
class Client;
class Channel;

class Bot {
	private:
		int 	_botFd;
		bool 	_sentBasicBotData;
		Client* _bot;

	public:
		Bot();
		~Bot();
		
		int const& 	getBotFd(void);
		bool const& hasSentBasicBotData(void);
		Client* getBot(void);
		void 	setBotFd(int botFd);
		void 	setSentBasicBotData(bool sentBasicBotData);
		
		void 	setBot(Client* bot);
		void 	setupBot(Server *server);
		void 	handleBotPollOut(int botFd, Server *server);
		int 	getBotFd() const;
		bool 	hasSentBasicBotData() const;
		void 	runBotCommand(Client* client, Channel* channel, std::string message);

		// Bot commands
		std::string getHelpMessage(const std::vector<std::string>& params);
		std::string diceGame(void);
		std::string flipGame(void);
		std::string trollGame(void);
		std::string slotGame(void);
		std::string rpsGame(const std::vector<std::string>& params);
		std::string handleWhoami(Client* client, Channel* channel);
		std::string handlePartCommand(Client* client, Channel* channel, const std::vector<std::string>& params);

};

#endif
