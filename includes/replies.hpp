
#ifndef REPLIES_HPP
# define REPLIES_HPP

# include "Includes.hpp"

// Utils
# define ERR_UNKNOWNCOMMAND(client, command)    HOST "421 " + client + " " + command + " :Unknown command"
# define ERR_NEEDMOREPARAMS(client, command)    HOST "461 " + client + " " + command + " :Not enough parameters"
# define ERR_ALREADYREGISTERED(client)          HOST "462 " + client + " :You may not reregister"
# define ERR_NOTREGISTERED(client)              HOST "451 " + client + " :You have not registered"
# define ERR_UNKNOWNERROR(client, message)      HOST "400 " + client + " :" + message

// RPL
# define RPL_WELCOME(fullUserId, client, serverName)            HOST "001 " + client + " :Welcome to the \"" + serverName + "\" Network," + fullUserId
# define RPL_YOURHOST(client, serverName)                       HOST "002 " + client + " :Your host is \"" + serverName + "\", running version 1.0"
# define RPL_CREATED(client, dateTime)                          HOST "003 " + client + " :This server was created at " + dateTime
# define RPL_ISUPPORT(client)                                   HOST "005 " + client + " NICKLEN=9 CHANNELLEN=200 TOPICLEN=307"

// CAP
# define RPL_CAPABILITIES(client, command, capabilitiesList)    HOST + client + " : CAP * " + command + " : [" + capabilitiesList + "]"
# define ERR_UNKNOWNCAP(client, command)                        HOST + client + " : CAP * " + command + " :Unknown CAP command"
# define RPL_ENDCAP(client)                                     HOST + client + " : CAP * END"

// INVITE
# define RPL_INVITE(fullUserId, client, channel)                fullUserId  + client + " " + client + " invites you to #" + channel
# define RPL_INVITING(client, invitedNickname, channel)         HOST "341 " + client + " " + invitedNickname + " #" + channel
# define ERR_USERONCHANNEL(client, invitedNickname, channel)    HOST "443 " + client + " " + invitedNickname + " #" + channel + " :is already on channel"

// JOIN
# define RPL_JOIN(fullUserId, channel)                  fullUserId + " JOIN #" + channel
# define RPL_NAMREPLY(client, channel, nicknamesList)   HOST "353 " + client + " = #" + channel + " :" + nicknamesList
# define RPL_ENDOFNAMES(client, channel)                HOST "366 " + client + " #" + channel + " :End of /NAMES list"

# define ERR_CHANNELISFULL(client, channel)             HOST "471 " + client + " #" + channel + " :Cannot join channel (+l)"
# define ERR_INVITEONLYCHAN(client, channel)            HOST "473 " + client + " #" + channel + " :Cannot join channel (+i)"
# define ERR_BADCHANNELKEY(client, channel)             HOST "476 " + client + " #" + channel + " :Cannot join channel (+k)"
# define ERR_BADCHANMASK(channel)                       HOST "476 " + channel + " :Bad Channel Mask"

// KICK
# define RPL_KICK(fullUserId, channel, kickedNickname, reason)  fullUserId + " KICK #" + channel + " " + kickedNickname + " :" + reason

# define ERR_NOSUCHCHANNEL(client, channel)                     HOST "403 " + client + " #" + channel + " :No such channel"
# define ERR_CHANOPRIVSNEEDED(client, channel)                  HOST "482 " + client + " #" + channel + " :You're not channel operator"
# define ERR_USERNOTINCHANNEL(client, destNickname, channel)    HOST "441 " + client + " " + destNickname + " #" + channel + " :They aren't on that channel"
# define ERR_CANTKICKSELF(fullUserID, client, channel)                      fullUserID + client + " #" + channel + " :You can't kick yourself"

// MODE
# define MODE_MSG(channel, mode)                                (std::string(HOST) + "MODE #" + (channel) + " " + (mode))
# define MODE_PARAM_MSG(channel, mode, param)                   HOST "MODE #" + channel + " " + mode + " " + param

# define ERR_INVALIDKEY(client, channel)                    HOST "525 " + client + " #" + channel + " " + " :Key is not well-formed"
# define ERR_INVALIDGIVENPASSWORD(client, channel, mode, password)      HOST "464 " + client + " #" + channel + " " + mode + " " + password + " :Wrong password format"
// # define ERR_INVALIDGIVENPASSWORD(password)                             HOST "464 " + client + " :Password incorrect"
# define ERR_INVALIDLIMITNUMBER(client, channel, mode, limit)           HOST "467 " + client + " #" + channel + " " + mode + " " + limit + " :Wrong user limit number"
// # define ERR_INVALIDLIMITNUMBER(client, channel, mode, limit)           fullUserId + client + " #" + channel + " " + mode + " " + limit + " :Wrong user limit number"
# define ERR_UMODEUNKNOWNFLAG(client)                                    HOST "501 " + client + " :Unknown MODE flag"

// NICK
# define RPL_NICK(fullUserId, newNickname)                  fullUserId + " You're now known as " + newNickname

# define ERR_NONICKNAMEGIVEN(client)                        HOST "431 " + client + " :No nickname given"
# define ERR_ERRONEUSNICKNAME(oldNickname, newNickname)     HOST "432 " + oldNickname + " " + newNickname + " :Erroneus nickname"
# define ERR_NICKNAMEINUSE(oldNickname, newNickname)        HOST "433 " + oldNickname + " " + newNickname + " :Nickname is already in use"

// PART
# define RPL_PART(fullUserId, channel, reason)  fullUserId + " PART #" + channel + " :" + reason

// PASS
# define ERR_PASSWDMISMATCH(client)     HOST "464 " + client + " :Password incorrect"

// PRIVMSG
# define RPL_PRIVMSG(fullUserId, destination, message)      fullUserId + " PRIVMSG " + destination + " :" + message

# define ERR_NOSUCHNICK(client, destination)    HOST "401 " + client + " " + destination + " :No such nick/channel"
# define ERR_NORECIPIENT(client)                HOST "411 " + client + " :No recipient given (PRIVMSG)"
# define ERR_NOTEXTTOSEND(client)               HOST "412 " + client + " :No text to send"
# define ERR_NOTONCHANNEL(client, channel)      HOST "442 " + client + " #" + channel + " :You're not on that channel"

// PING
# define RPL_PONG(fullUserId, token)    fullUserId + " PONG :" + token

// TOPIC
# define RPL_NOTOPIC(client, channel)                           HOST "331 " + client + " #" + channel + " :No topic is set"
# define RPL_TOPIC(client, channel, channelTopic)               HOST "332 " + client + " #" + channel + " :" + channelTopic
# define ERR_INVALIDTOPIC(fullUserId, client, channel)          fullUserId  + client + " #" + channel + " :Invalid topic"

// QUIT
# define RPL_QUIT(fullUserId, reason)   fullUserId + " QUIT :" + reason

#endif

// From https://modern.ircdocs.horse/