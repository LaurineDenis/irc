#ifndef PART_HPP
 #define PART_HPP

 #include <iostream>
 #include <string>
 #include <vector>
 #include "Irc.hpp"

class	Part
{
	public:

		Part();
		Part(Part const &cpy);
		~Part();
		Part					&operator=(Part const &src);
};

#endif
