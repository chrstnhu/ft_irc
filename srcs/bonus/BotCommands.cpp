#include "../../includes/bonus/BotBonus.hpp"

// Handle whoami command
std::string Bot::handleWhoami(Client* client, Channel* channel) {
	std::string whoMsg = "[WHOAMI] Nickname: " + client->getNick();
	if (channel->isOperator(client->getFd())) {
		whoMsg += ", you are an operator of this channel";
	} else {
		whoMsg += ", you are a regular user of this channel";
	}
	whoMsg += ", username: " + client->getUsername();
	whoMsg += ", realname: " + client->getRealname();
	whoMsg += ", joined this server at: " + getFormattedTime(client->getConnectionStartTime()) + ".";
	return whoMsg;
}

// Handle bot part command
std::string Bot::handlePartCommand(Client* client, Channel* channel, const std::vector<std::string>& params) {
	std::string reason;
	for (size_t i = 0; i < params.size(); i++) {
		reason += params[i] + " ";
	}
	if (reason.empty()) {
		reason = "No reason specified";
	}
	return RPL_PART(client->getFullUserId(), channel->getName(), reason + " (Removed by " + client->getNick() + ")") + CRLF;
}

// Get help message for bot commands
std::string Bot::getHelpMessage(const std::vector<std::string>& params) {
	std::string command;
	std::string output;

	if (params.size() >= 1) {
		command = params[0];
	}

	std::map<std::string, std::string> helpMessages;
	helpMessages["bot"]    = "[HELP BOT]: Usage: !bot, brings the BOT to the channel.";
	helpMessages["rpn"]    = "[HELP RPN]: Usage: !rpn <RPN formula>, calculates the result of the RPN formula and returns it.";
	helpMessages["dice"]   = "[HELP DICE]: Usage: !dice, rolls a 6-faces dice and returns the result.";
	helpMessages["flip"]   = "[HELP FLIP]: Usage: !flip, flips a coin and returns if it is Heads or Tails.";
	helpMessages["troll"]  = "[HELP TROLL]: Usage: !troll, ??? :)";
	helpMessages["rps"]    = "[HELP RPS]: Usage: !rps <rock|paper|scissors>, plays a game of rock-paper-scissors with the bot.";
	helpMessages["slot"]   = "[HELP SLOT]: Usage: !slot, plays a slot machine game with the bot.";
	helpMessages["whoami"] = "[HELP WHOAMI]: Usage: !whoami, gives informations about you.";
	helpMessages["part"]   = "[HELP PART]: Usage: !part, removes the BOT from the channel.";
	helpMessages["help"]   = "[HELP HELP]: Usage: !help <bot cmd>, returns the list of bot commands if cmd is not specified or the informations about the bot command elsewise.";

	if (!command.empty()) {
		std::map<std::string, std::string>::const_iterator it = helpMessages.find(command);
		if (it != helpMessages.end()) {
			output = it->second;
		} else {
			output = "Cannot find " BOLDDEF + command + DEF" help data.";
		}
	} else {
		output = "[HELP] [Bot commands list: bot, rpn, dice, flip, troll, rps, slot, whoami, part, help]";
	}
	return output;
}
