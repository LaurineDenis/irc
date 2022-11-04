#ifndef EXECUTIONMANAGER_HPP
# define EXECUTIONMANAGER_HPP

# include "Irc.hpp"

class	Client;
class	Channel;
class	Server;

enum	Command_lst
{
	PASS,
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

		ExecutionManager()	{};
		~ExecutionManager()	{};

		/* SETTER */

		void							setAddrlen(int len) {this->_addrlen = len;};

		/* GETTER */


		/* MEMBER FCTS */
		
		void		init(Server *server);
		void		addSd(int socket, short events);
		void		newClient(int newSocket);
		int			checkPoll();
		void		newConnection();
		void		deleteClient(int i); 
		void		init_cmd();
		void		sendRpl();
		void		dispatchCmd(Client *client, std::vector<std::string> line, int index, int cmd);
		void		parseCmd(Client *client, std::string buffer, int index);
		void		IO_Operation();
		std::string		recvCmd(int i);
		bool	parse_channel_name(std::string channel_name);
		void	command_pass(std::vector<std::string> out, Client *client);
		void	command_cap(std::vector<std::string> out);
		void	command_ping(std::vector<std::string> out);
		void	command_nick(std::vector<std::string> out, Client *client);
		void	command_client(std::vector<std::string> out, Client *client);
		void	command_join(std::vector<std::string> out, Client *client);
		void	command_privmsg(std::vector<std::string> out, Client *client);
		void	command_part(std::vector<std::string> out, Client *client);
		void	command_topic(std::vector<std::string> out, Client *client);
		void	command_kick(std::vector<std::string> out, Client *client);
		void	command_quit(Client *client, int index);
		void	change_topic(std::string topic, std::string client, Channel *channel);
		void	send_msg_to_channel_clients(std::string msg, Client *client, Channel *channel);
		void	send_msg_to_client(std::string msg, Client *other_client);
		void	print_infos();
		Client	*find_client(std::string nickname);
		Channel	*find_channel(std::string channel_name);
		void	delete_channel(Channel *channel);
		bool	check_nickname(std::string nickname);
		bool	is_operator(Client *client, Channel *channel);
		bool	is_register(Client *client);
		int		is_command(std::vector<std::string> line);
		void	remove_client_of_channel(Channel *channel, Client *client);
		void	shutdown();


	private:

		std::vector<Channel>		*_channels;
		std::vector<Client>			*_clients;
		std::vector<struct pollfd>	_clientSd;
		std::string					_password;
		std::string					_address;
		int							_port;
		int							_clientAmount;
		socklen_t					_addrlen;
		struct	sockaddr_in			_sockAdrr;
		std::vector<std::string>	*_cmd_name;
};

#endif
