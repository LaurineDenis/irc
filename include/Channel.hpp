#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Irc.hpp"

class	Client;

class	Channel
{
	public:

		Channel();
		Channel(Channel const &cpy);
		Channel(Client *creator, std::string name);
		Channel		&operator=(Channel const &cpy);
		~Channel();
		std::string				get_name();
		std::string				get_topic();
		std::string				get_topic_client();
		std::string				get_topic_time();
		Client					*get_operator();
		void					set_name(std::string name);
		void					set_topic(std::string topic);
		void					set_topic_client(std::string topic_client);
		void					set_topic_time(std::string topic_time);
		void					set_operator(Client *client);
		bool					banned_client(Client *client);
		bool					is_banned(Client *client);
		std::vector<Client>		*_clients;

	private:

		Client					*_operator;
		std::vector<Client>		*_banned;
		std::string				_name;
		std::string				_topic;
		std::string				_topic_client;
		std::string				_topic_time;
};

#endif
