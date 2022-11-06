#include "../../include/Irc.hpp"

Topic::Topic(void)
{
	std::cout << "Constructor Topic by default called" << std::endl;
}

Topic::Topic(Topic const &cpy)
{
	*this = cpy;
}

Topic::~Topic(void)
{
	std::cout << "Destructor Topic called" << std::endl;
}

Topic	&Topic::operator=(Topic const &src)
{
	return (*this);
}
