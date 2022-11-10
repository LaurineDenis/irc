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
			std::cout << "\tCreator is : " << it->get_creator()->get_nickname() << std::endl;
			if (!_channels->data()->_clients->empty())
			{
				std::cout << "\t\tClient in channel" << std::endl;
				for (std::vector<Client>::iterator it = _channels->data()->_clients->begin(); it != _channels->data()->_clients->end(); it++)
					std::cout << "\t\t\t" << it->get_nickname() << std::endl;
			}
		}
	}
	else
		std::cout << "\tNo Channels" << std::endl;
	std::cout << "Clients" << std::endl;
	if (_clients)
	{
		for (std::vector<Client>::iterator it = _clients->begin(); it != _clients->end(); it++)
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

void	ExecutionManager::send_msg_to_channel_clients(std::string msg, Client *user, Channel *channel)
{
	for (std::vector<Client>::iterator it_client = _clients->begin(); it_client != _clients->end(); it_client++)
	{
		for (std::vector<Channel>::iterator it_channel = it_client->_channels->begin(); it_channel != it_client->_channels->end(); it_channel++)
		{
			if (it_channel->get_name() == channel->get_name() && it_client->get_nickname() != user->get_nickname())
				it_client->answer = msg;
		}
	}
}

void	ExecutionManager::send_msg_to_all_clients_of_channel(std::string msg, Client *user, Channel *channel)
{
	for (std::vector<Client>::iterator it_client = _clients->begin(); it_client != _clients->end(); it_client++)
	{
		for (std::vector<Channel>::iterator it_channel = it_client->_channels->begin(); it_channel != it_client->_channels->end(); it_channel++)
		{
			if (it_channel->get_name() == channel->get_name())
				it_client->answer = msg;
		}
	}
}

void	ExecutionManager::command_pass(std::vector<std::string> out, Client *user)
{
	std::cout << "command PASS" << std::endl;
	if (out[1].empty())
		user->answer = ERR_NEEDMOREPARAMS(user->get_nickname());
	else if (out[1] != _password)
		user->answer = ERR_PASSWDMISMATCH(user->get_nickname());
	else
	{
		user->_pw = 1;
		return;
	}
	user->answer += "ERROR : Password mismatch";
	sendRpl();
	deleteClient(_clients->size() - 1);
	std::cout << _clients->size() << std::endl;
}

void	ExecutionManager::command_privmsg(std::vector<std::string> out, Client *user)
{
	std::cout << "command Privmsg" << std::endl;
	Channel		*channel;
	Client		*other_user;
	std::string	channel_name;
	std::string	msg;

	//channel case
	if (out[1][0] == '#')
	{
		channel_name = out[1];
		if ((channel = find_channel(channel_name)) == NULL)
			user->answer = out[1] + " No such channel" + ENDLINE;
		else
		{
			if (channel->is_moderated() && channel->is_voice_ok(user))
			{
				for (int i = 2; i < out.size(); i++)
					msg += " " + out[i];
				send_msg_to_channel_clients(":" + user->get_nickname() + "!" + user->get_name() + "@server PRIVMSG " + channel->get_name() + " " + msg + ENDLINE, user, channel);
			}
			else
				user->answer = ":server 404 " + user->get_nickname() + " " + channel->get_name() + " :Cannot send to channel" + ENDLINE;
		}
	}
	else if((other_user = find_client(out[1])) != NULL)
	{
		for (int i = 2; i < out.size(); i++)
				msg += " " + out[i];
		send_msg_to_client(":" + other_user->get_nickname() + "!" + other_user->get_name() + "@server PRIVMSG "+ user->get_nickname()+ " :" + msg + ENDLINE, other_user);
	}
	else
		user->answer = out[1] + " No such channel" + ENDLINE;
}

Channel	*ExecutionManager::find_channel(std::string channel_name)
{
	size_t		size;

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
