#include "../../include/Irc.hpp"

Part::Part(void)
{
	std::cout << "Constructor Part by default called" << std::endl;
}

Part::Part(Part const &cpy)
{
	*this = cpy;
}

Part::~Part(void)
{
	std::cout << "Destructor Part called" << std::endl;
}

Part	&Part::operator=(Part const &src)
{
	return (*this);
}
