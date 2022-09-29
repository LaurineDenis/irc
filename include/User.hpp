#ifndef USER_HPP
 #define USER_HPP

 #include <iostream>
 #include <string>
 #include <vector>
 #include "Irc.hpp"

class Channel;

class	User
{
	public:

		User();
		User(User const &cpy);
		~User();
		User					&operator=(User const &src);
		std::string				get_nickname();
		std::string				get_name();
		std::string				get_password();
		int						get_nb_channel();
		void					set_nb_channel(int nb_channel);
		void					set_nickname(std::string nickname);
		void					set_name(std::string name);
		void					set_password(std::string password);
		bool					wlcm_send;
		std::string				answer;
		std::vector<Channel>	*_channels;

	private:

		std::string				_nickname;
		std::string				_name;
		std::string				_password;
		int						_nb_channel;
};

#endif
