#ifndef INCLUDES_HPP
# define INCLUDES_HPP

# include <iostream>
# include <sstream>
# include <string>
# include <cstring>
# include <string>
# include <cstdlib>
# include <ctime>
# include <algorithm>

# include <map>
# include <vector>
# include <set>
# include <stack>

# include <signal.h>
# include <sys/types.h>
# include <sys/socket.h> // Socket
# include <netinet/in.h> // Network definitions: sockaddr_in, AF_INET, htons(), etc.
# include <arpa/inet.h> //提供与网络地址转换、IP 地址处理相关的函数，如 inet_pton、inet_ntoa。
# include <fcntl.h> // For F_SETFL and O_NONBLOCK
# include <poll.h>
# include <stdlib.h>
# include <ifaddrs.h>

# include "Colors.hpp"
# include "replies.hpp"
# include "Utils.hpp"

# define HOST ":localhost "

# define DEFAULT_NAME ""
# define BOT_NICKNAME "ft_ircbot"
# define BOT_USER "ft_ircbot 127.0.0.1 ft_irc :The almighty bot"
# define BOT_PREFIX '!'
# define BOT_NONE '\0'
# define BOT_JOIN 'J'
# define BOT_PART 'P'

# define CRLF "\r\n" ////IRC 协议规定的换行符
# define MESSAGE_SIZE 1024
# define CHANNEL_NAME_MAX_LEN 200
# define AUTO_LOGOUT_TIMEOUT 100
# define TOPIC_MAX_LEN 307

# define INVITE_ONLY_CHANNEL 'i'
# define RESTRICTED_TOPIC_CHANNEL 't'
# define CHANNEL_HAS_PASSWORD 'k'
# define CHANNEL_OPERATOR_PRIVILEGE 'o'
# define CHANNEL_IS_USER_LIMITED 'l'

# define APPEND '+'
# define ERASE '-'

# define MAX_TOPIC_LENGTH 255 // Max topic length (RFC 2812)

// Includes other hpp files
struct execReturnData {
    std::string message;
    std::vector<int> clientsFds;
};

struct IRCMessage {
    std::string prefix; 
    std::string command;
    std::vector<std::string> params;
    std::string text;
    bool hasText;
};

#endif