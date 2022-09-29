#include "../include/Irc.hpp"

std::vector<std::string> split_vector(std::string str, std::string delims)
{
	size_t						beg, pos = 0;
	std::vector<std::string>	token;

	while ((beg = str.find_first_not_of(delims, pos)) != std::string::npos)
	{
		pos = str.find_first_of(delims, beg + 1);
		token.push_back(str.substr(beg, pos - beg));
	}
	return (token);
}
