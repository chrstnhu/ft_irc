# ft_irc 🚀

A modern IRC (Internet Relay Chat) server implemented in C++98, compliant with RFC 1459 standards and developed as part of the 42 curriculum.

## 📋 Table of Contents

- [About](#about)
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Supported IRC Commands](#supported-irc-commands)
- [Bonus Bot](#bonus-bot)
- [Architecture](#architecture)
- [Testing](#testing)
- [Contributors](#contributors)

## 🎯 About

This project implements a complete IRC server allowing multiple clients to connect simultaneously and communicate through channels. The server handles authentication, channel creation, private and public messages, as well as various administrative commands.

**Key Features:**
- 🔐 Password authentication
- 👥 Multi-client support with non-blocking sockets
- 📢 Public and private channel management
- 🤖 Interactive bot with integrated games (bonus)
- 🛡️ Complete error handling and IRC response codes
- ⚡ Event-driven architecture with poll()

## ✨ Features

### Core Features
- **Multiple Connections**: Simultaneous handling of multiple clients
- **IRC Channels**: Channel creation, joining, and management
- **Messaging**: Private and public message sending
- **Moderation**: Administrative commands (KICK, MODE, TOPIC, etc.)
- **Authentication**: Password system to secure access

### Bonus Features
- **Interactive Bot** with game commands:
  - 🎲 Dice game (`!dice`)
  - 🪙 Coin flip (`!flip`)
  - ✂️ Rock-paper-scissors (`!rps`)
  - 🎰 Slot machine (`!slot`)
  - 🧮 RPN calculator (`!rpn`)
  - 👤 User information (`!whoami`)
  - 🎭 Mystery command (`!troll`)

## 🚀 Installation

1. **Clone the repository**
   ```bash
   git clone git@github.com:chrstnhu/ft_irc.git && cd ft_irc
   ```

2. **Compile the project**
   ```bash
   make
   ```

3. **Clean object files (optional)**
   ```bash
   make clean
   ```

## 🎮 Usage

### Starting the server

```bash
./ircserv <port> <password>
```

**Parameters:**
- `port`: Server listening port (e.g., 6667)
- `password`: Password for server connection

**Example:**
```bash
./ircserv 6667 mypassword
```

### Connecting with an IRC client

1. **With IRSSI:**
    ```bash
   /set nick yournickname
   /set username yourusername
   /connect localhost 6667 mypassword
   /join #channel
    ```
2. **With netcat (for testing):**
   ```bash
   nc localhost 6667
   ```

   Then send authentication commands:
   ```
   PASS mypassword
   NICK yournickname
   USER yournickname 0 * :Your Real Name
   ```

## 📋 Supported IRC Commands

### Authentication
- `PASS` - Set connection password
- `NICK` - Set or change nickname
- `USER` - Register user on server
- `CAP` - Client capability management

### Channel Management
- `JOIN` - Join a channel
- `PART` - Leave a channel
- `TOPIC` - View/modify channel topic
- `MODE` - Modify channel or user modes
- `KICK` - Kick a user from channel
- `INVITE` - Invite a user to channel

### Communication
- `PRIVMSG` - Send private or public message
- `PING` - Connectivity test
- `QUIT` - Disconnect from server

## 🤖 Bonus Bot

The server includes an interactive bot that automatically joins channels and offers various commands:

### Bot Commands

| Command | Description | Usage |
|---------|-------------|-------|
| `!help` | Show help | `!help [command]` |
| `!dice` | Roll a 6-sided die | `!dice` |
| `!flip` | Coin flip | `!flip` |
| `!rps` | Rock-paper-scissors | `!rps <rock\|paper\|scissors>` |
| `!slot` | Slot machine | `!slot` |
| `!rpn` | RPN calculator | `!rpn <formula>` |
| `!whoami` | User information | `!whoami` |
| `!part` | Remove bot from channel | `!part` |
| `!troll` | Mystery command | `!troll` |

### Bot Usage Examples

```irc
<user> !dice
<Bot> 🎲 [user] rolled a dice and got: 4!

<user> !rps rock
<Bot> 🪨📄✂️ [user] played Rock vs Bot's Paper - Bot wins! 🤖

<user> !rpn 3 4 + 2 *
<Bot> 🧮 [user] RPN calculation result: 14
```

## 👥 Contributors

[**chrhu**](https://github.com/chrstnhu) & [**xchen34**](https://github.com/xchen34)

---

## 📚 Resources

- [RFC 1459 - Internet Relay Chat Protocol](https://www.rfc-editor.org/rfc/rfc1459)
- [IRC Modern Replies](https://modern.ircdocs.horse/)
- [Socket](https://www.geeksforgeeks.org/c/socket-programming-cc/)
