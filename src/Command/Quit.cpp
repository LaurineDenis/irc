#include "../../include/Irc.hpp"

Quit::Quit(void)
{
	std::cout << "Constructor Quit by default called" << std::endl;
}

Quit::Quit(Quit const &cpy)
{
	*this = cpy;
}

Quit::~Quit(void)
{
	std::cout << "Destructor Quit called" << std::endl;
}

Quit	&Quit::operator=(Quit const &src)
{
	return (*this);
}
