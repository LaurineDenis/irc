#include "../include/Irc.hpp"

int ft_main(int ac, char **av)
{
	Server				*server = new Server();
	ExecutionManager	*exec = new ExecutionManager();

	server->init(ac, av);
	server->start(exec);
	exec->init(server);
	server->run(exec);
	delete server;
	delete exec;
	return 0;
}

int	main(int ac, char **av)
{
	ft_main(ac, av);
	while (1)
		;
		return 0;
}