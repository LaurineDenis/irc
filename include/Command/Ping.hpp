#ifndef PING_HPP
 #define PING_HPP

 #include <iostream>
 #include <string>
 #include <vector>
 #include "Irc.hpp"

class	Ping
{
	public:

		Ping();
		Ping(Ping const &cpy);
		~Ping();
		Ping					&operator=(Ping const &src);
};

#endif
