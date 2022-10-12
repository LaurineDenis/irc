#include "../include/Server.hpp"

void	ExecutionManager::change_topic(std::string topic, std::string user, Channel *channel)
{
	// char			time_test[300];
	time_t	timestamp = time( NULL );
    // struct tm * pTime = localtime( & timestamp );
	// strftime(time_test, 300, "%a, %d %h %G %T", pTime);
	// std::string		time_string;
	channel->set_topic(topic);
	channel->set_topic_user(user);
	channel->set_topic_time(ctime(&timestamp));
}

void	ExecutionManager::command_topic(std::vector<std::string> out, User *user)
{
	std::cout << "command TOPIC" << std::endl;
	Channel		*channel;
	std::string	channel_name;

	if (out[1][0] == '#')
	{
		channel_name = out[1].erase(0, 1);
		if ((channel = find_channel(channel_name)) == NULL)
			user->answer = ":server 403 " + out[1] + " No such channel" + ENDLINE;
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
				user->answer = ":server 461 " +  user->get_name() + " TOPIC :Not enough parameters" + ENDLINE;
			else
			{
				change_topic(out[2].erase(0, 1), user->get_name(), channel);
				user->answer = ":server 332 " +  user->get_name() + " #" + channel->get_name() + " :" + channel->get_topic() + ENDLINE;
			}
		}
	}
	else
	{
		user->answer = ":server 403 " + out[1] + " No such channel" + ENDLINE;
	}
}


void	ExecutionManager::command_join(std::vector<std::string> out, User *user, std::string str)
{
	std::cout << "command Join" << std::endl;
	Channel						*channel;
	std::string					channel_name;
	std::vector<std::string>	split_comma;
	int							i = 0;
	std::string					msg = "";

	// std::cout << out.size() << std::endl;
	str.erase(0,5);
	split_comma = split_vector(str, "\r\n,");
	for (std::vector<std::string>::iterator it = split_comma.begin(); it != split_comma.end(); ++it)
		std::cout << "|" << *it << "|" << std::endl;
	while (i < split_comma.size())
	{
		std::cout << "channel join = " << split_comma[i] << std::endl;
		if (split_comma[i][0] != '#')
		{
			std::cout << "# OU PAS " << split_comma[i][0] << std::endl;
			user->answer += ":server 403 " + split_comma[i] + " No such channel" + ENDLINE;			
			i++;
		}
		else
		{
			//sortir boucle si espace pas voulu
			std::cout << "channel join = " << split_comma[i] << std::endl;
			if (split_comma[i - 1][split_comma[i - 1].size() - 1] == ' ')
				break;
			channel_name = split_comma[i].erase(0, 1);
			if ((channel = find_channel(channel_name)) == NULL)
			{
				//creer le channel si il existe pas correspondant
				channel = new Channel(user, channel_name);
				//add channel in server
				std::cout << "push back name = " << channel->get_name() << std::endl;
				_channels->push_back(*channel);
			}
			if (add_channel_in_user(channel, user) == false)
			{
				user->answer = ":server 474 " + user->get_nickname() + " " + channel->get_name() + " :Cannot join channel (+b)" + ENDLINE;
				return ;
			}
			user->answer += ":" + user->get_nickname() + "!" + user->get_nickname() + "@server JOIN #" + channel->get_name() + ENDLINE;
			if (channel->get_topic() != "")
				user->answer += ":server 332 " +  user->get_name() + " #" + channel->get_name() + " :" + channel->get_topic() + ENDLINE + ":server 333 " +  user->get_name() + " #" + channel->get_name() + " " + channel->get_topic_user() + " " + channel->get_topic_time() + ENDLINE;
			if (channel->_users->size() > 1)
			{
				for (int i = 0; i < channel->_users->size(); i++)
				{
					if (i != 0)
						msg += " ";
					if (channel->_users->at(i).get_nickname() == channel->get_operator()->get_nickname())
						msg+= "@";
					msg += channel->_users->at(i).get_nickname();
				}
				user->answer += ":server 353 " +  user->get_name() + " = #" + channel->get_name() + " :" + msg + ENDLINE;
			}
			send_msg_to_channel_users(":" + user->get_nickname() + "!" + user->get_nickname() + "@server JOIN #" + channel->get_name() + ENDLINE, user, channel);
			i++;
		}
	}
	if (i != split_comma.size())
	{
		user->answer = ":server 403 " + out[i - 1] + " No such channel" + ENDLINE;
	}
}

bool	is_in_channel(Channel *channel, User *user)
{
	for (std::vector<User>::iterator ite = channel->_users->begin(); ite != channel->_users->end(); ite++)
		if (ite->get_nickname() == user->get_nickname())
			return (true) ;
	return (false);
}

bool	add_channel_in_user(Channel *channel, User *user)
{
	//add channel in user
	if (channel->is_banned(user) == true)
		return (false);
	user->_channels->push_back(*channel);
	user->set_nb_channel(user->get_nb_channel() + 1);
	if (is_in_channel(channel, user) == false)
		channel->_users->push_back(*user);
	return (true);
}

void	ExecutionManager::delete_channel(Channel *channel)
{
	for (size_t i = 0; i < _channels->size(); i++)
	{
		if (channel->get_name() == _channels->at(i).get_name())
		{
			_channels->erase(_channels->begin() + i);
			break ;
		}
	}
}

void	ExecutionManager::remove_user_of_channel(Channel *channel, User *user)
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
		if (ite->get_nickname() == user->get_nickname())
			break ;
	channel->_users->erase(channel->_users->begin() + i);
	//supprimer le channel si 0 users
	if (channel->_users->empty())
		delete_channel(channel);
}

void	ExecutionManager::command_part(std::vector<std::string> out, User *user)
{
	std::cout << "command Part" << std::endl;
	Channel		*channel;
	std::string	channel_name;

	if (out[1][0] == '#')
	{
		channel_name = out[1].erase(0, 1);
		if ((channel = find_channel(channel_name)) == NULL)
		{
			//Le channel n'existe pas => Pas de message d'erreurs ?
			std::cout << "No Channel found" << std::endl;
			user->answer = ":server 403 " + out[1] + " No such channel" + ENDLINE;
		}
		else
		{
			//quit le channel qui existe deja
			remove_user_of_channel(channel, user);
			user->answer = ":" + user->get_nickname() + "!" + user->get_name() + "@server PART #" + channel->get_name() + ENDLINE;
			std::cout << "send = " << user->answer << std::endl;
			send_msg_to_channel_users(":" + user->get_nickname() + "!" + user->get_nickname() + "@server PART #" + channel->get_name() + ENDLINE, user, channel);
		}
	}
	else
	{
		user->answer = ":server 403 " + out[1] + " No such channel" + ENDLINE;
	}
}

bool	ExecutionManager::is_operator(User *user, Channel *channel)
{
	if (user->get_nickname() == channel->get_operator()->get_nickname())
		return (true);
	return (false);
}

void	ExecutionManager::command_kick(std::vector<std::string> out, User *user)
{
	std::cout << "command Kick" << std::endl;
	Channel		*channel;
	std::string	channel_name;

	if (out[1][0] == '#')
	{
		channel_name = out[1].erase(0, 1);
		if ((channel = find_channel(channel_name)) == NULL)
		{
			//Le channel n'existe pas => Pas de message d'erreurs ?
			std::cout << "No Channel found" << std::endl;
			user->answer = ":server 403 " + out[1] + " No such channel" + ENDLINE;
		}
		else
		{
			if (is_operator(user, channel) == true)
			{
				User	*userToBan;
				userToBan = find_user(out[2]);
				// remove user from channel
				remove_user_of_channel(channel, userToBan);
				// add user in banned list
				channel->banned_user(userToBan);
				// send msg to user banned
				user->answer = ":" + user->get_nickname() + "!" + user->get_name() + "@server KICK #" + channel->get_name() + " " + out[2] + ENDLINE;
				// send msg to user who banned
				send_msg_to_user(user->answer, userToBan);
			}
			else
			{
				user->answer = ":server 482 " + user->get_nickname() + " #" + out[1] + " :You're not channel operator" + ENDLINE;
			}
		}
	}
	else
	{
		user->answer = ":server 403 " + out[1] + " No such channel" + ENDLINE;
	}
}
