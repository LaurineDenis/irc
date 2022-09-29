#include "../include/Irc.hpp"

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
			std::cerr << "Error: Wrong port" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	server->_pw = pw;
	server->_port = std::stod(port);
	server->_sockAddress.sin_family = AF_INET;
	server->_sockAddress.sin_addr.s_addr = INADDR_ANY;
	server->_sockAddress.sin_port = htons(server->_port);
	return;
}

void	Server::addSd(int socket, short events)
{
	struct pollfd	add = {socket, events, 0};

	this->_clientSd.push_back(add);
}

void	Server::startServer(Server *server)
{
	int	opt = 1;

	if ((server->_masterSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "socket failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (setsockopt(server->_masterSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt))
	{
		std::cerr << "setsockopt failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (bind(server->_masterSocket, reinterpret_cast<struct sockaddr *>(&server->_sockAddress), sizeof(server->_sockAddress)) < 0)
	{
		std::cerr << "bind failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (listen(server->getMasterSocket(), 50) < 0)
	{
		std::cerr << "listen failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	server->_addrlen = sizeof(server->_address);
	std::cout << "Waiting for connections ... " << std::endl;
	server->_users = new std::vector<User>;
	server->_channels = new std::vector<Channel>;
	server->_clientFd.push_back(server->_masterSocket);
	server->addSd(server->_masterSocket, POLLIN);
	server->set_address("127.0.0.1");
}

void	Server::newUser(Server *server, int newSocket)
{
	User	*user = new User();

	server->_clientAmount++;
	fcntl(newSocket, F_SETFL, O_NONBLOCK);
	server->_users->push_back(*user);
	server->_clientFd.push_back(newSocket);
	server->addSd(newSocket, POLLIN); /* MIGHT CHANGE */
}

void	Server::run(Server *server)
{
	struct	sockaddr_in	newClient;
	socklen_t			addrSize;
	int					events;
	int					newSocket;
	int					valread;
	char				buffer[1024];
	char				c = 0;

	while (TRUE)
	{
		events = 0;
		if ((events = poll(server->_clientSd.data(), server->_clientSd.size(), -1)) < 0)
		{
			std::cerr << "Poll failed" << std::endl;
			exit(0);
		}
		for (int i = 0; i < server->_clientAmount; i++)
		{
			if (server->_clientSd.at(i).revents & POLLIN && i == 0)
			{
				std::cout << "=================================" << std::endl;
				addrSize = sizeof(server->_sockAddress);
				newSocket = accept(server->_masterSocket, (struct sockaddr *)&newClient, &addrSize);
				std::cout << "New Connection: " << newSocket << std::endl;
				server->newUser(server, newSocket);
				server->_clientSd.at(i).revents = 0;
			}
			else
			{
				valread = 0;
				while ((read(server->_clientFd.at(i), &c, 1)) > 0 && c != '\n')
				{
					buffer[valread] = c;
					valread++;
				}
				if (valread == 0 && server->_clientSd.at(i).revents == POLLHUP)
					std::cerr << "User disconnected" << std::endl; // a changer je sais 
				else if (valread > 0)
				{
					buffer[valread] = 0;
					std::cout << "Client " << server->_clientFd.at(i) << " sent:" << buffer << std::endl;
					dispatch_cmd(buffer, server, &server->_users->at(i - 1));
				}
				std::cout << server->_clientAmount << std::endl;
				for (int j = 1; j < server->_clientAmount; j++)
				{
					if (i > 0 && server->_users->at(j - 1).answer != "")
					{
						std::string answer = server->_users->at(j - 1).answer + ENDLINE;
						std::cout << answer.c_str() << std::endl;
						send(server->_clientFd.at(j), answer.c_str(), answer.length(), 0);
						server->_users->at(j - 1).answer = "";
					}
				}
			}
			server->_clientSd.at(i).revents = 0;
		}
		for (int i = 0; i < server->_clientAmount; i++)
			server->_clientSd.at(i).revents = 0;
	}
}
