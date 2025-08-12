# Define colors
GREEN = \033[0;32m
YELLOW = \033[0;33m
DEFAULT = \033[0m

# Path to libraries
OBJ_DIR	= obj
SRCS_PATH = srcs
COMMANDS_PATH = srcs/commands
UTILS_PATH = srcs/utils
BONUS_PATH = srcs/bonus
HEADERS = includes

# Source files
SRCS_FILES = Server.cpp Client.cpp Channel.cpp Command.cpp
COMMANDS_FILES = NICK.cpp PASS.cpp USER.cpp CAP.cpp JOIN.cpp PING.cpp \
	QUIT.cpp PART.cpp KICK.cpp INVITE.cpp MODE.cpp TOPIC.cpp \
	PRIVMSG.cpp 
UTILS_FILES = printMsg.cpp utils.cpp
BONUS_FILES = BotGames.cpp BotCommands.cpp BotBonus.cpp RPN.cpp 

SRCS = $(SRCS_FILES:%=$(SRCS_PATH)/%) $(COMMANDS_FILES:%=$(COMMANDS_PATH)/%) \
	$(UTILS_FILES:%=$(UTILS_PATH)/%) $(SRCS_PATH)/main.cpp $(BONUS_FILES:%=$(BONUS_PATH)/%)


OBJS = $(SRCS:$(SRCS_PATH)/%.cpp=$(OBJ_DIR)/%.o)

# Compilation
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -Wshadow

NAME = ircserv

# Create a objet directory
$(OBJ_DIR)/%.o: $(SRCS_PATH)/%.cpp $(HEADERS)/*.hpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ 
	
# Execute
all: $(NAME)

$(NAME) : $(OBJS) $(HEADERS)/*.hpp
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)$(NAME) has been created!$(DEFAULT)"

clean :
	rm -f $(OBJS)
	rm -rf $(OBJ_DIR)
	@echo "$(YELLOW)Object files have been removed!$(DEFAULT)"

fclean : clean
	rm -f $(NAME)
	@echo "$(YELLOW)$(NAME) has been removed!$(DEFAULT)"

re: fclean all

.PHONY: all clean fclean re