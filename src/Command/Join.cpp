#include "../../include/Irc.hpp"

Join::Join(void)
{
	std::cout << "Constructor Join by default called" << std::endl;
}

Join::Join(Join const &cpy)
{
	*this = cpy;
}

Join::~Join(void)
{
	std::cout << "Destructor Join called" << std::endl;
}

Join	&Join::operator=(Join const &src)
{
	return (*this);
}

void	Join::execute(std::vector<std::string> line, Client *client, ExecutionManager *exec)
{
	std::cout << "command Join" << std::endl;
}
