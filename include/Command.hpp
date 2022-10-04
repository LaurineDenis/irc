// #ifndef COMMAND_HPP
// # define COMMAND_HPP

// #include "Irc.hpp"

// class Server;

// enum	Command_lst
// {
// 	NICK,
// 	USER,
// 	CAP,
// 	JOIN,
// 	PRIVMSG,
// 	PART,
// 	TOPIC,
// 	KICK,
// 	PING,
// 	QUIT
// };

// class Command
// {
// 	public:

// 			Command();
// 			~Command();
// 	void	command_cap(std::vector<std::string> out);
// 	void	command_ping(std::vector<std::string> out);
// 	void	command_nick(std::vector<std::string> out, User *user, Server *server);
// 	void	command_user(std::vector<std::string> out, Server *server, User *user);
// 	void	command_join(std::vector<std::string> out, User *user, Server *server);
// 	void	command_privmsg(std::vector<std::string> out, User *user, Server *server);
// 	void	command_part(std::vector<std::string> out, User *user, Server *server);
// 	void	command_topic(std::vector<std::string> out, User *user, Server *server);
// 	void	command_kick(std::vector<std::string> out, User *user, Server *server);
// 	void	command_quit(std::vector<std::string> out, User *user, Server *server);
// 	void	change_topic(std::string topic, std::string user, Channel *channel);
// 	void	send_msg_to_channel_users(std::string msg, User *user, Channel *channel, Server *server);

// };

// #endif
