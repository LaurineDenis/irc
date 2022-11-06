#ifndef KICK_HPP
 #define KICK_HPP

 #include <iostream>
 #include <string>
 #include <vector>
 #include "Irc.hpp"

class	Kick
{
	public:

		Kick();
		Kick(Kick const &cpy);
		~Kick();
		Kick					&operator=(Kick const &src);
};

#endif
