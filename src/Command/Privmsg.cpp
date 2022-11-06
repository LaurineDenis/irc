#include "../../include/Irc.hpp"

Privmsg::Privmsg(void)
{
	std::cout << "Constructor Privmsg by default called" << std::endl;
}

Privmsg::Privmsg(Privmsg const &cpy)
{
	*this = cpy;
}

Privmsg::~Privmsg(void)
{
	std::cout << "Destructor Privmsg called" << std::endl;
}

Privmsg	&Privmsg::operator=(Privmsg const &src)
{
	return (*this);
}
