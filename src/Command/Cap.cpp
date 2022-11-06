#include "../../include/Irc.hpp"

Cap::Cap(void)
{
	std::cout << "Constructor Cap by default called" << std::endl;
}

Cap::Cap(Cap const &cpy)
{
	*this = cpy;
}

Cap::~Cap(void)
{
	std::cout << "Destructor Cap called" << std::endl;
}

Cap	&Cap::operator=(Cap const &src)
{
	return (*this);
}

void	Cap::execute(std::vector<std::string> line, Client *client, ExecutionManager *exec)
{
	std::cout << "command CAP" << std::endl;
}
