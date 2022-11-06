#ifndef PRIVMSG_HPP
 #define PRIVMSG_HPP

 #include <iostream>
 #include <string>
 #include <vector>
 #include "Irc.hpp"

class	Privmsg
{
	public:

		Privmsg();
		Privmsg(Privmsg const &cpy);
		~Privmsg();
		Privmsg					&operator=(Privmsg const &src);
};

#endif
