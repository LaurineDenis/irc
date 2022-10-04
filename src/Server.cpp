#include "../include/Irc.hpp"

void	Server::init(int ac, char **av)
{
	std::string	pw;
	std::string port;

	if (ac != 3)
	{
		std::cerr << "Error: \"./ircserv <port> <pw>\"" << std::endl;
		exit(0);
	}
	pw = av[2];
	port = av[1];
	this->_pw = pw;
	this->_port = std::stod(port);
	if (this->_port > 65535 || this->_port < 1024) // a check
	{
		std::cerr << " 1024 < <port> < 65535" << std::endl;
	}
	return;
}

void		Server::start(ExecutionManager *exec)
{
	int					sockets;
	int					opt = 1;
	struct sockaddr_in	addr = {AF_INET, INADDR_ANY, htons(this->_port)};

	if ((sockets = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "socket failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (setsockopt(sockets, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt))
	{
		std::cerr << "setsockopt failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (bind(sockets, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0)
	{
		std::cerr << "bind failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (listen(sockets, 50) < 0)
	{
		std::cerr << "listen failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	exec->setAddrlen(sizeof(addr));
	this->_socket = sockets;
	std::cout << "Waiting for connections ... " << std::endl;
}

void	Server::run(ExecutionManager *exec)
{
	int		events;

	/* ADD PASSWORD REQUEST */
	while (TRUE)
	{
		events = exec->checkPoll();
		if (events < 0)
		{
			std::cerr << "Poll failed" << std::endl;
			exit(EXIT_FAILURE);
		}
		else if (events == 1)
			exec->newConnection();
		else
			exec->IO_Operation();
	}
}
