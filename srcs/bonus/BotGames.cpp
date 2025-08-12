#include "../../includes/bonus/BotBonus.hpp"

// Dice game (dice roll)
std::string Bot::diceGame(void) {
	int dice = rand() % 6 + 1;
	return "[DICE] The dice result is: " + numberToString(dice) + " !";
}

// Flip game (coin flip)
std::string Bot::flipGame(void) {
	int dice = rand() % 2;
	if (dice == 0) {
		return "[FLIP] The flip result is: Heads !";
	} else {
		return "[FLIP] The flip result is: Tails !";
	}
	return "[FLIP] Error while flipping the coin, please try again later.";
}

// RPS game (rock-paper-scissors)
std::string Bot::rpsGame(const std::vector<std::string>& params) {
	std::string rps;
	if (params.empty()) {
		rps = "[RPS] Usage: !rps <rock|paper|scissors>";
	} else {
		std::string userChoice = params[0];
		std::string choices[] = { "rock", "paper", "scissors" };
		std::string botChoice = choices[rand() % 3];
		std::string emojis[] = { "✊", "✋", "✌️" };

		int userIndex = -1;
		int botIndex = -1;
		for (int i = 0; i < 3; ++i) {
			if (choices[i] == userChoice) {
				userIndex = i;
			}
			if (choices[i] == botChoice) {
				botIndex = i;
			}
		}

		if (userIndex == -1) {
			rps = "[RPS] Invalid choice. Use: rock, paper, or scissors.";
			return rps;
		}
		std::string result = "[RPS] You chose " + emojis[userIndex] 
						+ ", I chose " + emojis[botIndex] + ". ";

		if (userChoice == botChoice) {
			result += "It's a draw!";
		} else if ((userChoice == "rock" && botChoice == "scissors") ||
				(userChoice == "paper" && botChoice == "rock") ||
				(userChoice == "scissors" && botChoice == "paper")) {
			result += "You win!";
		} else {
			result += "I win!";
		}
		rps = result;
	}
	return rps;
}

// Slot game (simple slot machine with 3 symbols)
std::string Bot::slotGame(void) {
	std::string slot = "[SLOT] ";
	const char* symbols[] = { "🍒", "🍋", "🍊", "🔔", "⭐" };
	std::string s1 = symbols[rand() % 5];
	std::string s2 = symbols[rand() % 5];
	std::string s3 = symbols[rand() % 5];

	slot = "| " + s1 + " | " + s2 + " | " + s3 + " | ";
	if (s1 == s2 && s2 == s3)
		slot += "Jackpot! 🎉";
	else if (s1 == s2 || s2 == s3 || s1 == s3)
		slot += "Nice! You got a pair!";
	else
		slot += "Better luck next time!";
	return slot;
}

// Troll game (random troll message)
std::string Bot::trollGame(void) {
	static const std::string trolls[] = {
		"Segmentation fault (Core trolled)",
		"Fatal error: Keyboard not found. Press F1 to continue.",
		"Error: You've been trolled 🧌",
		"Program exited with status: LOL",
		"404: Troll not found",
		"Exception: Troll not found in the wild",
		"Syntax error: Unexpected troll",
		"Memory leak detected: Trolls are everywhere",
	};
	int index = rand() % (sizeof(trolls) / sizeof(trolls[0]));
	return std::string(RED) + trolls[index] + DEF;
}
