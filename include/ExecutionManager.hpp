#ifndef EXECUTIONMANAGER_HPP
# define EXECUTIONMANAGER_HPP

# include "Irc.hpp"

class	ExecutionManager {

	public:

		/* CONSTRUCTOR && DESTRUCTOR */

		ExecutionManager()								{};
		~ExecutionManager()								{};

		/* SETTER */

		int							setAddrlen(int len) {this->_addrlen = len;};

		/* GETTER */


		/* MEMBER FCTS */
		
		void		init(Server *server);
		void		addSd(int socket, short events);
		void		newUser(int newSocket);
		int			checkPoll();
		void		newConnection();
		void		deleteUser(int i); 
		void		sendRpl();
		void		dispatchCmd();
		void		IO_Operation();
		std::string		recvCmd(int i);

	private:

		std::vector<Channel>		_channels;
		std::vector<User>			_users;
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
