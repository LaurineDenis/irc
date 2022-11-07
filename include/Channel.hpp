#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Irc.hpp"

class	Client;

class	Channel
{
	public:

		Channel();
		Channel(Channel const &cpy);
		Channel(Client *creator, std::string name, std::string time);
		Channel		&operator=(Channel const &cpy);
		~Channel();
		std::string				get_name();
		std::string				get_topic();
		std::string				get_topic_client();
		std::string				get_topic_time();
		Client					*get_creator();
		std::string		get_time();
		void					set_name(std::string name);
		void					set_topic(std::string topic);
		void					set_topic_client(std::string topic_client);
		void					set_topic_time(std::string topic_time);
		void					set_creator(Client *client);
		void					set_time(std::string time);
		bool					add_operator(Client *client);
		bool					remove_operator(Client *client);
		bool					is_operator(Client *client);
		bool					add_banned(Client *client);
		bool					remove_banned(Client *client);
		bool					is_banned(Client *client);
		bool					add_invited(Client *client);
		bool					remove_invited(Client *client);
		bool					is_invited(Client *client);
		bool					add_voice_ok(Client *client);
		bool					remove_voice_ok(Client *client);
		bool					is_voice_ok(Client *client);
		bool					is_invite_only();
		bool					is_mode_topic();
		bool					is_moderated();
		void					change_invite_only(bool invite_only);
		void					change_mode_topic(bool mode_topic);
		void					change_moderated(bool moderated);
		std::vector<Client>		*_clients;

	private:

		Client					*_creator;
		std::vector<Client>		*_operator;
		std::vector<Client>		*_banned;
		std::vector<Client>		*_invited;
		std::vector<Client>		*_voice_ok;
		std::string				_name;
		std::string				_topic;
		std::string				_topic_client;
		std::string				_topic_time;
		bool					_invite_only;
		bool					_mode_topic;
		bool					_moderated;
		std::string		_time;
};

#endif
