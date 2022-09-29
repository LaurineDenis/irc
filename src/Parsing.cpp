#include "Irc.hpp"

void	checkParams(Server *server, int ac, char **av)
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
	server->setPw(pw);
	server->setPort(stoi(port));
}
