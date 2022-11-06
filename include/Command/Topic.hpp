#ifndef TOPIC_HPP
 #define TOPIC_HPP

 #include <iostream>
 #include <string>
 #include <vector>
 #include "Irc.hpp"

class	Topic
{
	public:

		Topic();
		Topic(Topic const &cpy);
		~Topic();
		Topic					&operator=(Topic const &src);
};

#endif
