#ifndef CLIENT_HPP
 #define CLIENT_HPP

 #include <iostream>
 #include <string>
 #include <vector>
 #include "Irc.hpp"

class Channel;

class	Client
{
	public:

		Client();
		Client(Client const &cpy);
		~Client();
		Client					&operator=(Client const &src);
		std::string				get_nickname();
		std::string				get_name();
		std::string				get_password();
		int						get_nb_channel();
		void					set_nb_channel(int nb_channel);
		void					set_nickname(std::string nickname);
		void					set_name(std::string name);
		void					set_password(std::string password);
		void					set_checkPw(bool i);
		bool					wlcm_send;
		std::string				answer;
		std::vector<Channel>	*_channels;

	private:

		std::string				_nickname;
		std::string				_name;
		std::string				_password;
		int						_nb_channel;
		bool					_checkPw;
};

#endif
