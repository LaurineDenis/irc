#include "../../include/Irc.hpp"

User::User(void)
{
	std::cout << "Constructor User by default called" << std::endl;
}

User::User(User const &cpy)
{
	*this = cpy;
}

User::~User(void)
{
	std::cout << "Destructor User called" << std::endl;
}

User	&User::operator=(User const &src)
{
	return (*this);
}
