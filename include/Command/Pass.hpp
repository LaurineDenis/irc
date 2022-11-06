#ifndef PASS_HPP
 #define PASS_HPP

 #include <iostream>
 #include <string>
 #include <vector>
 #include "Irc.hpp"

class	Pass
{
	public:

		Pass();
		Pass(Pass const &cpy);
		~Pass();
		Pass					&operator=(Pass const &src);
};

#endif
