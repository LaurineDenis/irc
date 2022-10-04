#ifndef SERVER_HPP
# define SERVER_HPP

# include "Irc.hpp"

class	User;
class	Channel;

class	Server {

	public:

		/* CONSTRUCTOR  DESTRUCTOR */

		Server()								{};
		~Server()								{};

		/* GETTER & SETTER */

		int		getSocket()						{return (this->_socket);};

		/* MEMBER FCTS */

		void	init(int ac, char **av);
		void	start(ExecutionManager *exec);
		void	run(ExecutionManager *exec);

	private:

		std::string			_pw;
		int					_port;
		int					_socket;
};

#endif
