#include "../include/Irc.hpp"

int main(int ac, char **av)
{
	Server	*server = new Server();

	server->initServer(server, ac, av);
	server->startServer(server);
	server->run(server);
}
