#ifndef NUMERICREPLY_HPP
#define NUMERICREPLY_HPP

#define ENDLINE 										"\r\n"
#define SERVER_NAME										"ft_irc"
#define SERVER_VERSION									"1.0"

// ERROR REPLIES 1459
#define ERR_NOSUCHNICK(nickname)						"401 " + nickname + " :No such nick/channel" + ENDLINE
#define ERR_UNKNOWNCOMMAND(command)						"421 " + command + " :Unknown command" + ENDLINE
#define ERR_NONICKNAMEGIVEN								"431 :No nickname given\n\r"
#define ERR_ERRONEUSNICKNAME(nickname)					"432 " + nickname + " :Erroneous nickname" + ENDLINE		
#define ERR_NICKNAMEINUSE(nickname)						"433 " + nickname + " :Nickname is already in use" + ENDLINE
#define ERR_NOTONCHANNEL(channel)						"442 " + channel + " :You're not on that channel" + ENDLINE
#define ERR_INVALIDUSERNAME								"468 * USER :Your username is not valid\n\r"
#define ERR_UNKNOWNMODE(char)							"472 " + char + " :is unknown mode char to me" + ENDLINE
#define ERR_CHANOPRIVSNEEDED(channel)					"482 " + channel + " :You're not channel operator" + ENDLINE

// ERROR REPLIES 2812
#define ERR_NOSUCHCHANNEL(channel)						"403 " + channel + " :No such channel" + ENDLINE
#define ERR_CANNOTSENDTOCHAN(channel)					"404 " + channel + " :Cannot send to channel" + ENDLINE
#define ERR_NORECIPIENT(command)						"411 :No recipient given " + command + ENDLINE
#define ERR_USERNOTINCHANNEL(nickname, channel)			"441 " + nickname + " " + channel + " :They aren't on that channel" + ENDLINE
#define ERR_NOTONCHANNEL(channel)						"442 " + channel + " :You're not on that channel" + ENDLINE
#define ERR_USERONCHANNEL(nickname, channel)			"443 " + nickname + " " + channel + " :is already on channel" + ENDLINE
#define ERR_NOTREGISTERED								"451 :You have not registrered\r\n"
#define ERR_NEEDMOREPARAMS(command)						"461 " + command + " :Not enough parameters" + ENDLINE
#define ERR_ALREADYREGISTRED							"462 :Unauthorized command (already registered)\r\n"
#define ERR_PASSWDMISMATCH(nickname)					"464 " + nickname + " :Password incorrect\r\n"
#define ERR_INVITEONLYCHAN(channel)						"473 " + channel + " :Cannot join channel (+i)" + ENDLINE
#define ERR_BANNEDFROMCHAN(channel)						"474 " + channel + " :Cannot join channel (+b)" + ENDLINE
#define ERR_BADCHANNAME(channel)						"479 " + channel + " :Invalid channel name" + ENDLINE
#define ERR_NOPRIVILEGES								"481 :Permission Denied- You're not an IRC operator\r\n"

// COMMAND RESPONSES RFC_2812
#define RPL_WELCOME(nickname)							"001 " + nickname + " :Welcome to the wonderful ft_irc serveur " + nickname + ENDLINE
#define RPL_YOURHOST(nickname)							"002 " + nickname + " :Your host is ft_irc, running version 1.0" + ENDLINE
#define RPL_LUSERCLIENT(nickname, number_of_clients)	"251 " + nickname + " :There are " + number_of_clients +  " users on 1 servers" + ENDLINE
#define RPL_LUSERCHANNELS(nickname, number_of_channels)	"254 " + nickname + " :There are " + number_of_channels +  " channels formed" + ENDLINE
#define RPL_CHANNELMODEIS(nickname, channel, mode, mode_params)	"324 " + nickname + " " + channel + " :" + mode + mode_params + ENDLINE
#define RPL_NOTOPIC(nickname, channel) 					"331 " + nickname + " " + channel + " :No topic is set" + ENDLINE
#define RPL_TOPIC(nickname, channel, topic) 			"332 " + nickname + " " + channel + " :" + topic + ENDLINE
#define RPL_TOPICWHOTIME(nickname, channel, topic_client, topic_time) "333 " + nickname + " " + channel + " " + topic_client + " " + topic_time + ENDLINE
#define RPL_NAMREPLY(channel, nickname, members)		"353 " + nickname + " " + channel + " :" + members + ENDLINE
#define RPL_INVITING(inviting, channel, invited)		"341 " + inviting + " " + invited + " " + channel + ENDLINE
#define RPL_ENDOFNAMES(channel, nickname)				"366 " + nickname + " " + channel + " :End of /NAMES list" + ENDLINE
#define RPL_MOTD(nickname, message)						"372 " + nickname + " :" + message + ENDLINE
#define RPL_ENDOFMOTD(nickname)							"376 " + nickname + " :End of message of the day." + ENDLINE
#define RPL_ENDOFBANLIST(nickname, channel)             "368 " + nickname + " " + channel + ENDLINE
#define RPL_BANLIST(nickname, channel, lst)             "367 " + nickname + " " + channel + " " + lst + ENDLINE;

// MSG_BUILDER
#define MSG_JOIN(channel, nickname)						":" + nickname + " JOIN " + channel + ENDLINE
#define MSG_PART(channel, nickname, part_msg)			":" + nickname + " PART " + channel +  " " + part_msg + ENDLINE
#define MSG_KICK(operator, channel, kicked, kick_msg)	":" + operator + " KICK " + channel +  " " + kicked + " " + kick_msg + ENDLINE
#define MSG_INVITE(inviting, invited, channel)			":" + inviting + " INVITE " + invited +  " " + channel + ENDLINE
#define MSG_NICK(old_nickname, new_nickname)			":" + old_nickname + " NICK " + new_nickname + ENDLINE
#define MSG_MODE(nickname)								":" + nickname + " MODE " + nickname + " :+w" + ENDLINE
#define MSG_KILL(nickname, message)						":bot KILL " + nickname + " " + message + ENDLINE
#define MSG_PRIVMSG(sender, recipient, msg)				":" + sender + " PRIVMSG " + recipient + " " + msg + ENDLINE
#define MSG_NOTICE(sender, recipient, msg)				":" + sender + " NOTICE " + recipient + " " + msg + ENDLINE
#define MSG_MODE_DETAILS(nickname, channel, mode, params) ":" + nickname +" MODE " + channel + " " + mode + params + ENDLINE


// #define MSG_NOTICE(rpl, sending_nickname, recipient, msg)	rpl + ":" + sending_nickname + " NOTICE " + recipient + " :" + msg + ENDLINE


#endif // NUMERIC_REPLIES_HPP
