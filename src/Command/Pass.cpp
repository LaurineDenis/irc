#include "../../include/Irc.hpp"

Pass::Pass(void)
{
	std::cout << "Constructor Pass by default called" << std::endl;
}

Pass::Pass(Pass const &cpy)
{
	*this = cpy;
}

Pass::~Pass(void)
{
	std::cout << "Destructor Pass called" << std::endl;
}

Pass	&Pass::operator=(Pass const &src)
{
	return (*this);
}
