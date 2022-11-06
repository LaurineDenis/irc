#ifndef NICK_HPP
 #define NICK_HPP

 #include <iostream>
 #include <string>
 #include <vector>
 #include "Irc.hpp"

class	Nick
{
	public:

		Nick();
		Nick(Nick const &cpy);
		~Nick();
		Nick					&operator=(Nick const &src);
};

#endif
