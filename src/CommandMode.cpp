#include "../include/Irc.hpp"

void	ExecutionManager::command_mode(std::vector<std::string> line, Client *client)
{
	std::cout << "command Mode" << std::endl;

	Channel				*channel;
	std::string			msg;
	std::string			str;

	if (line.size() >= 2)
		str = (line.begin() + 1)->data();
	if (line.size() < 2)
		client->answer += ERR_NOSUCHCHANNEL(str);
	else if ((channel = find_channel(str)) == NULL)
		client->answer += ERR_NOSUCHCHANNEL(str);
	else
	{
		std::string		mode = "itomvb";
		std::size_t		pos;
		if (check_mode(client, channel, line) == true)
		{
			str = (line.begin() + 2)->data();
			if ((pos = mode.find_first_of(str[1], 0)) != std::string::npos)
				select_mode(client, channel, line, pos);
			else
				client->answer = ERR_UNKNOWNMODE(str);
		}
	}
}

void	ExecutionManager::select_mode(Client *client, Channel *channel, std::vector<std::string> line, std::size_t pos)
{
	void		(ExecutionManager::*function[])
	(Client *client, Channel *channel, std::vector<std::string> line) = 
	{&ExecutionManager::mode_invite, &ExecutionManager::mode_topic, &ExecutionManager::mode_operator, &ExecutionManager::mode_moderated, &ExecutionManager::mode_voice, &ExecutionManager::mode_banned};

	(this->*function[pos])(client, channel, line);
}

bool	ExecutionManager::check_mode(Client *client, Channel *channel, std::vector<std::string> line)
{
	Client	*other_client;
	
	if (is_in_channel(channel, client))
	{
		if (line.size() >= 3)
		{
			if (channel->is_operator(client))
			{
				if (line.at(2).at(0) == '+' || line.at(2).at(0) == '-')
				{
					if (line.size() >= 4 && (line.at(2).at(1) == 'o' || line.at(2).at(1) == 'v' || line.at(2).at(1) == 'b'))
					{
						if ((other_client = find_client(line.at(3))) != NULL && (is_in_channel(channel, other_client) || line.at(2).at(1) == 'b'))
							return (true);
						else
							client->answer += ERR_USERNOTINCHANNEL(client->get_nickname(), channel->get_name());
					}
					else if (line.at(2).at(1) == 'o' || line.at(2).at(1) == 'v')
						client->answer += ERR_NEEDMOREPARAMS(line.at(0));
					else
						return (true);
				}
				else
					client->answer += ERR_UNKNOWNMODE(line.at(2));
			}
			else
				client->answer += ERR_CHANOPRIVSNEEDED(channel->get_name());
		}
		else
		{
			std::string	mode_lst;
			mode_lst += "+";
			if (channel->is_invite_only())
				mode_lst += "i";
			if (channel->is_moderated())
				mode_lst += "m";
			if (channel->is_mode_topic())
				mode_lst += "t";
			client->answer += RPL_CHANNELMODEIS(client->get_nickname(), channel->get_name(), mode_lst, "");
		}
	}
	else
		client->answer += ERR_USERNOTINCHANNEL(client->get_nickname(), channel->get_name());
	return (false);
}

void	ExecutionManager::mode_invite(Client *client, Channel *channel, std::vector<std::string> line)
{
	std::cout << "Mode Invite" << std::endl;
    if (line.at(2).at(0) == '+')
	{
        channel->change_invite_only(true);
		send_msg_to_all_clients_of_channel(MSG_MODE_DETAILS(client->get_nickname(), channel->get_name(), "+i", ""), client, channel);
	}
    else if (line.at(2).at(0) == '-')
	{
        channel->change_invite_only(false);
		send_msg_to_all_clients_of_channel(MSG_MODE_DETAILS(client->get_nickname(), channel->get_name(), "-i", ""), client, channel);
	}
}

void	ExecutionManager::mode_topic(Client *client, Channel *channel, std::vector<std::string> line)
{
	std::cout << "Mode Topic" << std::endl;
    if (line.at(2).at(0) == '+')
	{
        channel->change_mode_topic(true);
		send_msg_to_all_clients_of_channel(MSG_MODE_DETAILS(client->get_nickname(), channel->get_name(), "+t", ""), client, channel);
	}
    else if (line.at(2).at(0) == '-')
	{
        channel->change_mode_topic(false);
		send_msg_to_all_clients_of_channel(MSG_MODE_DETAILS(client->get_nickname(), channel->get_name(), "-t", ""), client, channel);
	}
}

void	ExecutionManager::mode_moderated(Client *client, Channel *channel, std::vector<std::string> line)
{
	std::cout << "Mode Moderated" << std::endl;
    if (line.at(2).at(0) == '+')
	{
        channel->change_moderated(true);
		send_msg_to_all_clients_of_channel(MSG_MODE_DETAILS(client->get_nickname(), channel->get_name(), "+m", ""), client, channel);
	}
    else if (line.at(2).at(0) == '-')
	{
        channel->change_moderated(false);
		send_msg_to_all_clients_of_channel(MSG_MODE_DETAILS(client->get_nickname(), channel->get_name(), "-m", ""), client, channel);
	}
}

void	ExecutionManager::mode_operator(Client *client, Channel *channel, std::vector<std::string> line)
{
	std::cout << "Mode Operator" << std::endl;
    Client  *other_client;

    if (line.at(2).at(0) == '+')
    {
        other_client = find_client(line.at(3).data());
	    channel->add_operator(other_client);
		send_msg_to_all_clients_of_channel(MSG_MODE_DETAILS(client->get_nickname(), channel->get_name(), " +o ", other_client->get_nickname()), client, channel);
    }
    else
    {
		other_client = find_client(line.at(3).data());
		channel->remove_operator(other_client);
		send_msg_to_all_clients_of_channel(MSG_MODE_DETAILS(client->get_nickname(), channel->get_name(), " -o ", other_client->get_nickname()), client, channel);
	}
	for (unsigned long i = 0; i < _clients->size(); i++)
	{
		if(is_in_channel(channel, &_clients->at(i)))
			send_list_name_channel(&_clients->at(i), channel);
	}
	/* send_list_name_channel(client, channel); */
	/* send_list_name_channel(other_client, channel); */
}

void	ExecutionManager::mode_voice(Client *client, Channel *channel, std::vector<std::string> line)
{
	std::cout << "Mode Voice" << std::endl;
	Client  *other_client;

    if (line.at(2).at(0) == '+')
    {
        other_client = find_client(line.at(3).data());
        channel->add_voice_ok(other_client);
		client->answer += MSG_MODE_DETAILS(client->get_nickname(), channel->get_name(), " +v ", other_client->get_nickname());
		other_client->answer += MSG_MODE_DETAILS(client->get_nickname(), channel->get_name(), " +v ", other_client->get_nickname());
    }
    else if (line.at(2).at(0) == '-')
    {
		other_client = find_client(line.at(3).data());
		channel->remove_voice_ok(other_client);
		client->answer += MSG_MODE_DETAILS(client->get_nickname(), channel->get_name(), " -v ", other_client->get_nickname());
		other_client->answer += MSG_MODE_DETAILS(client->get_nickname(), channel->get_name(), " -v ", other_client->get_nickname());
	}
}

void	ExecutionManager::mode_banned(Client *client, Channel *channel, std::vector<std::string> line)
{
	std::cout << "Mode Banned" << std::endl;
	Client  *other_client;
  
	if (line.size() > 3)
	{
		if (line.at(2).at(0) == '+')
		{
			other_client = find_client(line.at(3).data());
			channel->add_banned(other_client);
			client->answer += MSG_MODE_DETAILS(client->get_nickname(), channel->get_name(), " +b ", other_client->get_nickname());
		}
		else if (line.at(2).at(0) == '-')
		{
			other_client = find_client(line.at(3).data());
			channel->remove_banned(other_client);
			client->answer += MSG_MODE_DETAILS(client->get_nickname(), channel->get_name(), " -b ", other_client->get_nickname());
		}
	}
	else if (line.size() == 3)
	{
		std::string	msg;
		msg = channel->list_banned(client->get_nickname());
		client->answer +=  msg + RPL_ENDOFBANLIST(client->get_nickname(), channel->get_name());
	}
	else
		client->answer += ERR_NEEDMOREPARAMS(line.at(0));
}
