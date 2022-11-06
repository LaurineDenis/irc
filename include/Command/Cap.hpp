#ifndef CAP_HPP
 #define CAP_HPP

 #include <iostream>
 #include <string>
 #include <vector>
 #include "Irc.hpp"

class	Cap
{
	public:

		Cap();
		Cap(Cap const &cpy);
		~Cap();
		Cap					&operator=(Cap const &src);

	private :
		void	execute(std::vector<std::string> line, Client *client, ExecutionManager *exec);
};

#endif
