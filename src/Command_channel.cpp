#include "../include/Server.hpp"

void	Command::change_topic(std::string topic, std::string user, Channel *channel)
{
	// char			time_test[300];
	time_t timestamp = time( NULL );
    // struct tm * pTime = localtime( & timestamp );
	// strftime(time_test, 300, "%a, %d %h %G %T", pTime);
	// std::string		time_string;
	channel->set_topic(topic);
	channel->set_topic_user(user);
	channel->set_topic_time(ctime(&timestamp));
}

void	Command::command_topic(std::vector<std::string> out, User *user, Server *server)
{
	std::cout << "command TOPIC" << std::endl;
	Channel		*channel;
	std::string	channel_name;

	if (out[1][0] == '#')
	{
		channel_name = out[1].erase(0, 1);
		if ((channel = find_channel(server, channel_name)) == NULL)
			user->answer = out[1] + " No such channel" + ENDLINE;
		else
		{
			std::cout << "Topic Time == " << channel->get_topic_time() << std::endl;
			if (out.size() <= 2)
			{
				if (channel->get_topic() == "")
					user->answer = ":server 331 " +  user->get_name() + " #" + channel->get_name() + " :No topic is set" + ENDLINE;
				else
					user->answer = ":server 332 " +  user->get_name() + " #" + channel->get_name() + " :" + channel->get_topic() + ENDLINE + ":server 333 " +  user->get_name() + " #" + channel->get_name() + " " + channel->get_topic_user() + " " + channel->get_topic_time() + ENDLINE;
			}
			else if (out[2][0] != ':')
				user->answer = "ERR_NEEDMOREPARAMS";
			else
			{
				change_topic(out[2].erase(0, 1), user->get_name(), channel);
				user->answer = ":server 332 " +  user->get_name() + " #" + channel->get_name() + " :" + channel->get_topic() + ENDLINE;
			}
		}
	}
	else
	{
		user->answer = out[1] + " No such channel" + ENDLINE;
	}
}

void	Command::command_join(std::vector<std::string> out, User *user, Server *server)
{
	std::cout << "command Join" << std::endl;
	Channel		*channel;
	std::string	channel_name;
	int			i = 1;

	std::cout << out.size() << std::endl;
	while (i < out.size() && out[i][0] =='#')
	{
		std::cout << "channel join = " << out[i] << std::endl;
		channel_name = out[i].erase(0, 1);
		if ((channel = find_channel(server, channel_name)) == NULL)
		{
			//creer le channel si il existe pas correspondant
			channel = new Channel(user, channel_name);
			//add channel in server
			std::cout << "push back name = " << channel->get_name() << std::endl;
			server->_channels->push_back(*channel);
		}
		add_channel_in_user(channel, user);
		user->answer += ":" + user->get_nickname() + "!" + user->get_nickname() + "@server JOIN #" + channel->get_name() + ENDLINE;
		i++;
	}
	if (i != out.size())
	{
		user->answer = out[i - 1] + " No such channel" + ENDLINE;
	}
}

void	add_channel_in_user(Channel *channel, User *user)
{
	//add channel in user
	user->_channels->push_back(*channel);
	user->set_nb_channel(user->get_nb_channel() + 1);
}

void	delete_channel(Channel *channel, Server *server)
{
	for (size_t i = 0; i < server->_channels->size(); i++)
	{
		if (channel->get_name() == server->_channels->at(i).get_name())
		{
			server->_channels->erase(server->_channels->begin() + i);
			break ;
		}
	}
}

void	remove_user_of_channel(Channel *channel, User *user, Server *server)
{
	int		i = 0;
	//supprimer le channel dans user
	for (std::vector<Channel>::iterator ite = user->_channels->begin(); ite != user->_channels->end(); ite++, i++)
		if (ite->get_name() == channel->get_name())
			break ;
	user->_channels->erase(user->_channels->begin() + i);
	//supprimer le user dans channel
	i = 0;
	for (std::vector<User>::iterator ite = channel->_users->begin(); ite != channel->_users->end(); ite++, i++)
		if (ite->get_name() == user->get_name())
			break ;
	channel->_users->erase(channel->_users->begin() + i);
	//supprimer le channel si 0 users
	if (channel->_users->empty())
		delete_channel(channel, server);
}

void	Command::command_part(std::vector<std::string> out, User *user, Server *server)
{
	std::cout << "command Part" << std::endl;
	Channel		*channel;
	std::string	channel_name;

	if (out[1][0] == '#')
	{
		channel_name = out[1].erase(0, 1);
		if ((channel = find_channel(server, channel_name)) == NULL)
		{
			//Le channel n'existe pas => Pas de message d'erreurs ?
			std::cout << "No Channel found" << std::endl;
			user->answer = out[1] + " No such channel" + ENDLINE;
		}
		else
		{
			//quit le channel qui existe deja
			remove_user_of_channel(channel, user, server);
			user->answer = ":" + user->get_nickname() + "!" + user->get_name() + "@server PART #" + channel->get_name() + ENDLINE;
			std::cout << "send = " << user->answer << std::endl;
		}
	}
	else
	{
		user->answer = out[1] + " No such channel" + ENDLINE;
	}
}

void	Command::command_kick(std::vector<std::string> out, User *user, Server *server)
{
	std::cout << "command Kick" << std::endl;
	Channel		*channel;
	
	std::string	channel_name;

	if (out[1][0] == '#')
	{
		channel_name = out[1].erase(0, 1);
		if ((channel = find_channel(server, channel_name)) == NULL)
		{
			//Le channel n'existe pas => Pas de message d'erreurs ?
			std::cout << "No Channel found" << std::endl;
			user->answer = out[1] + " No such channel" + ENDLINE;
		}
		else
		{
			
			user->answer = ":" + user->get_nickname() + "!" + user->get_name() + "@server PART #" + channel->get_name() + ENDLINE;
		}
	}
	else
	{
		user->answer = out[1] + " No such channel" + ENDLINE;
	}
}
