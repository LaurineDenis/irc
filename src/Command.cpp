#include "../include/Server.hpp"

void	ExecutionManager::print_infos()
{
	std::cout << "\n-------Print Data-------" << std::endl;
	std::cout << "Channels" << std::endl;
	if (_channels->size() != 0)
	{
		for (std::vector<Channel>::iterator it = _channels->begin(); it != _channels->end(); it++)
		{
			std::cout << "\t" << it->get_name() << std::endl;
			std::cout << "\tOperator is : " << it->get_operator()->get_nickname() << std::endl;
			if (!_channels->data()->_users->empty())
			{
				std::cout << "\t\tUser in channel" << std::endl;
				for (std::vector<User>::iterator it = _channels->data()->_users->begin(); it != _channels->data()->_users->end(); it++)
					std::cout << "\t\t\t" << it->get_nickname() << std::endl;
			}
		}
	}
	else
		std::cout << "\tNo Channels" << std::endl;
	std::cout << "Users" << std::endl;
	if (_users)
	{
		for (std::vector<User>::iterator it = _users->begin(); it != _users->end(); it++)
		{
			std::cout << "\t" << it->get_nickname() << std::endl;
			if (it->get_nb_channel() != 0)
			{
				std::cout << "\t\tChannel in user" << std::endl;
				for (std::vector<Channel>::iterator ite = it->_channels->begin(); ite != it->_channels->end(); ite++)
					std::cout << "\t\t\t" << ite->get_name() << std::endl;
			}
		}
	}
	std::cout << "-------End Print-------\n" << std::endl;
}

void	ExecutionManager::send_msg_to_channel_users(std::string msg, User *user, Channel *channel)
{
	for (std::vector<User>::iterator it = _users->begin(); it != _users->end(); it++)
	{
		//envoyer le message si l'user n'est pas en mode off ??
		for (std::vector<Channel>::iterator ite = it->_channels->begin(); ite != it->_channels->end(); ite++)
		{
			if (ite->get_name() == channel->get_name() && it->get_nickname() != user->get_nickname())
			{
				it->answer = msg;
				std::cout << it->answer << std::endl;
			}
		}
	}
}

void	ExecutionManager::command_privmsg(std::vector<std::string> out, User *user)
{
	std::cout << "command Privmsg" << std::endl;
	Channel		*channel;
	std::string	channel_name;
	std::string	msg;

	//channel case
	if (out[1][0] == '#')
	{
		channel_name = out[1].erase(0, 1);
		if ((channel = find_channel(channel_name)) == NULL)
		{
			//Le channel n'existe pas
			user->answer = out[1] + " No such channel" + ENDLINE;
		}
		else
		{
			for (int i = 2; i < out.size(); i++)
				msg += " " + out[i];
			send_msg_to_channel_users(":" + user->get_nickname() + "!" + user->get_name() + "@server PRIVMSG #" + channel->get_name() + " " + msg + ENDLINE, user, channel);
		}
	}
	//user case
	else
	{
		user->answer = out[1] + " No such channel" + ENDLINE;
	}
}

Channel	*ExecutionManager::find_channel(std::string channel_name)
{
	size_t		size;
	// size_t		i;

	if (!_channels)
		return (NULL);
	size = _channels->size();
	for (size_t i = 0; i < size; i++)
	{
		if (channel_name == _channels->at(i).get_name())
			return (&_channels->at(i));
	}
	return (NULL);
}
