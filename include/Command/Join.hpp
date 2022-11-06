#ifndef JOIN_HPP
 #define JOIN_HPP

 #include <iostream>
 #include <string>
 #include <vector>
 #include "Irc.hpp"

class	Join
{
	public:

		Join();
		Join(Join const &cpy);
		~Join();
		Join					&operator=(Join const &src);

	private :
		void	execute(std::vector<std::string> line, Client *client, ExecutionManager *exec);
};

#endif
