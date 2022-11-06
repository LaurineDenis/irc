#include "../../include/Irc.hpp"

Kick::Kick(void)
{
	std::cout << "Constructor Kick by default called" << std::endl;
}

Kick::Kick(Kick const &cpy)
{
	*this = cpy;
}

Kick::~Kick(void)
{
	std::cout << "Destructor Kick called" << std::endl;
}

Kick	&Kick::operator=(Kick const &src)
{
	return (*this);
}
