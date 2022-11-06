#ifndef QUIT_HPP
 #define QUIT_HPP

 #include <iostream>
 #include <string>
 #include <vector>
 #include "Irc.hpp"

class	Quit
{
	public:

		Quit();
		Quit(Quit const &cpy);
		~Quit();
		Quit					&operator=(Quit const &src);
};

#endif
