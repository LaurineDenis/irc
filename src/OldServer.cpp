#include "Server.hpp"

void	Server::initServer(Server *server, int ac, char **av)
{ 
	std::string pw;
	std::string port;

	if (ac != 3)
	{
		std::cout << "Error: \"./ircserv <port> <password>\"" << std::endl;
		exit(0);
	}
	pw = av[2];
	port = av[1];
	if (!pw.length())
	{
		std::cout << "Error: Wrong password" << std::endl;
		exit(0);
	}
	for (int i = 0; port[i]; i++)
	{
		if (!isdigit(port[i]))
		{
			std::cout << "Error: Wrong port" << std::endl;
			exit(0);
		}
	}
	server->_pw = pw;
	server->_port = std::stod(port);
	server->_address.sin_family = AF_INET;
	server->_address.sin_addr.s_addr = INADDR_ANY;
	server->_address.sin_port = htons(server->_port);
	return;
}

void	Server::startServer(Server *server)
{
	if ((server->_masterSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		std::cerr << "socket failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (bind(server->_masterSocket, reinterpret_cast<struct sockaddr *>(&server->_address), sizeof(server->_address)) < 0)
	{
		std::cerr << "bind failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (listen(server->getMasterSocket(), 3) < 0)
	{
		std::cerr << "listen failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	server->_addrlen = sizeof(server->_address);
	std::cout << "Waiting for connections ..." << std::endl;
	std::cout << "Addrlen = " << server->_addrlen << std::endl;
	std::cout << "PW = " << server->_pw << std::endl;
	std::cout << "Port = " << server->_port << std::endl;
}

void	Server::run(Server *server)
{
	while (TRUE) 
	{
		FD_ZERO(&server->_readfds);
		FD_SET(server->_mastersocket, &server->_readfds);
		server->_maxSd = server->_mastersocket;
		/* AJOUT CLIENT, SOCKET, CMD CAP */

	}
}
