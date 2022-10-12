#ifndef SERVER_HPP
# define SERVER_HPP

# include "Irc.hpp"

class	ExecutionManager;

class	Server {

	public:

		/* CONSTRUCTOR  DESTRUCTOR */

		Server()								{};
		~Server()								{};

		/* GETTER & SETTER */

		int				getSocket()						{return (this->_socket);};
		std::string		getPw()							{return (this->_pw);};

		/* MEMBER FCTS */

		void			init(int ac, char **av);
		void			start(ExecutionManager *exec);
		void			run(ExecutionManager *exec);
		static void		signalHandler(int sig);

	private:

		std::string			_pw;
		int					_port;
		int					_socket;
};

#endif
