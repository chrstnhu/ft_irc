# Define colors
GREEN = \033[0;32m
YELLOW = \033[0;33m
DEFAULT = \033[0m

# Path to libraries
OBJ_DIR	= obj
SRCS_PATH = srcs
COMMANDS_PATH = srcs/commands
UTILS_PATH = srcs/utils
HEADERS = includes

# Source files
SRCS_FILES = 
COMMANDS_FILES = 
UTILS_FILES = print_msg.cpp

SRCS = $(SRCS_FILES:%=SRCS_PATH/%) $(COMMANDS_FILES:%=COMMANDS_PATH/%) \
	$(UTILS_FILES:%=UTILS_PATH/%) \
	srcs/main.c

OBJS = $(SRCS:$(SRCS_PATH)/%.c=$(OBJ_DIR)/%.o)

# Compilation
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -Wshadow

NAME = ircserv

# Create a objet directory
$(OBJ_DIR)/%.o: $(SRCS_PATH)/%.cpp $(HEADERS)/*.hpp
	@mkdir -p $(OBJ_DIR) 
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