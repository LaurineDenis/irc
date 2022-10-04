#ifndef EXECUTIONMANAGER_HPP
# define EXECUTIONMANAGER_HPP

# include "Irc.hpp"

class	User;
class	Channel;
class	Server;

enum	Command_lst
{
	NICK,
	USER,
	CAP,
	JOIN,
	PRIVMSG,
	PART,
	TOPIC,
	KICK,
	PING,
	QUIT
};

class	ExecutionManager {

	public:

		/* CONSTRUCTOR && DESTRUCTOR */

		ExecutionManager()								{};
		~ExecutionManager()								{};

		/* SETTER */

		void							setAddrlen(int len) {this->_addrlen = len;};

		/* GETTER */


		/* MEMBER FCTS */
		
		void		init(Server *server);
		void		addSd(int socket, short events);
		void		newUser(int newSocket);
		int			checkPoll();
		void		newConnection();
		void		deleteUser(int i); 
		void		sendRpl();
		void		dispatchCmd(User *user, std::string buffer);
		void		IO_Operation();
		std::string		recvCmd(int i);
		void	command_cap(std::vector<std::string> out);
		void	command_ping(std::vector<std::string> out);
		void	command_nick(std::vector<std::string> out, User *user);
		void	command_user(std::vector<std::string> out, User *user);
		void	command_join(std::vector<std::string> out, User *user, std::string str);
		void	command_privmsg(std::vector<std::string> out, User *user);
		void	command_part(std::vector<std::string> out, User *user);
		void	command_topic(std::vector<std::string> out, User *user);
		void	command_kick(std::vector<std::string> out, User *user);
		void	command_quit(std::vector<std::string> out, User *user);
		void	change_topic(std::string topic, std::string user, Channel *channel);
		void	send_msg_to_channel_users(std::string msg, User *user, Channel *channel);
		void	send_msg_to_user(std::string msg, User *other_user);
		void	print_infos();
		User	*find_user(std::string nickname);
		Channel	*find_channel(std::string channel_name);
		void	delete_channel(Channel *channel);
		bool	check_nickname(std::string nickname);
		bool	is_operator(User *user, Channel *channel);
		void	remove_user_of_channel(Channel *channel, User *user);


	private:

		std::vector<Channel>		*_channels;
		std::vector<User>			*_users;
		std::vector<struct pollfd>	_clientSd;
		std::vector<int>			_clientFd;
		std::string					_password;
		std::string					_address;
		int							_port;
		int							_clientAmount;
		socklen_t					_addrlen;
		struct	sockaddr_in			_sockAdrr;

};

#endif
