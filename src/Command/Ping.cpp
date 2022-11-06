#include "../../include/Irc.hpp"

Ping::Ping(void)
{
	std::cout << "Constructor Ping by default called" << std::endl;
}

Ping::Ping(Ping const &cpy)
{
	*this = cpy;
}

Ping::~Ping(void)
{
	std::cout << "Destructor Ping called" << std::endl;
}

Ping	&Ping::operator=(Ping const &src)
{
	return (*this);
}
