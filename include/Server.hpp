#ifndef SERVER_HPP
# define SERVER_HPP

# include "Irc.hpp"

class	User;
class	Channel;

class	Server {

	public:

		/* CONSTRUCTOR  DESTRUCTOR */

		Server() : _clientAmount(1)				{};
		~Server()								{};
		
		 /* SETTER */

		void	setPw(std::string pw)			{_pw = pw;};
		void	setPort(int port)				{_port = port;};
		void	setMasterSocket(int i)			{_masterSocket = i;};
		void	setAddrlen(int len)				{_addrlen = len;};
		void	setMaxSd(int sd)				{_maxSd = sd;};

		 /* GETTER */
		
		std::string	getPw()						{return _pw;};
		int			getPort()					{return _port;};
		sockaddr_in	getAddress()				{return _sockAddress;};
		int			getMasterSocket()			{return _masterSocket;};
		int			getMaxSd()					{return _maxSd;};
		int			getClientAmount()			{return _clientAmount;};

		/* MEMBER FCTS */

		void	startServer(Server *server);
		void	initServer(Server *server, int ac, char **av);
		void	run(Server *server);
		void	addSd(int socket, short events);
		void	newUser(Server *server, int newSocket);
		std::string				get_address()	{return _address;};
		void					set_address(std::string addr)	{_address = addr;};
		User					*init_data(Server *server);
		std::vector<Channel>	*_channels;
		std::vector<User>   	*_users;

	private:

		std::string			_pw;
		int					_port;
		int					_masterSocket;
		struct	sockaddr_in	_sockAddress;
		std::string			_address;
		int					_addrlen;
		int					_maxSd;
		int					_clientAmount;
		fd_set						_readFd;
		std::vector<struct	pollfd>	_clientSd;
		std::vector<int>			_clientFd;

};

#endif
