#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Irc.hpp"

class	User;

class	Channel
{
	public:

		Channel();
		Channel(Channel const &cpy);
		Channel(User *creator, std::string name);
		Channel		&operator=(Channel const &cpy);
		~Channel();
		std::string				get_name();
		std::string				get_topic();
		std::string				get_topic_user();
		std::string				get_topic_time();
		User					*get_operator();
		void					set_name(std::string name);
		void					set_topic(std::string topic);
		void					set_topic_user(std::string topic_user);
		void					set_topic_time(std::string topic_time);
		void					set_operator(User *user);
		std::vector<User>		*_users;

	private:

		User					*_operator;
		std::vector<User>		*_banned;
		std::string				_name;
		std::string				_topic;
		std::string				_topic_user;
		std::string				_topic_time;
};

#endif
