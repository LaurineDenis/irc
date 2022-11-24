#include "../include/Irc.hpp"

int main(int ac, char **av)
{
	Server				*server = new Server();
	ExecutionManager	*exec = new ExecutionManager();

	server->init(ac, av);
	server->start(exec);
	exec->init(server);
	server->run(exec);
	/* delete server; */
	/* delete exec; */
}
