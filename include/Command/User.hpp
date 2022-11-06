#ifndef USER_HPP
 #define USER_HPP

 #include <iostream>
 #include <string>
 #include <vector>
 #include "Irc.hpp"

class	User
{
	public:

		User();
		User(User const &cpy);
		~User();
		User					&operator=(User const &src);
};

#endif
