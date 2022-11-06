#include "../../include/Irc.hpp"

Nick::Nick(void)
{
	std::cout << "Constructor Nick by default called" << std::endl;
}

Nick::Nick(Nick const &cpy)
{
	*this = cpy;
}

Nick::~Nick(void)
{
	std::cout << "Destructor Nick called" << std::endl;
}

Nick	&Nick::operator=(Nick const &src)
{
	return (*this);
}
