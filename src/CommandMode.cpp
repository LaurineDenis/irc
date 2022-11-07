#include "../include/Irc.hpp"

void	ExecutionManager::command_mode(std::vector<std::string> line, Client *client)
{
	std::cout << "command Mode" << std::endl;

	Channel				*channel;
	std::string			msg;

	//Affichage
	for (std::vector<std::string>::iterator it = line.begin(); it != line.end(); ++it)
		std::cout << "|" << *it << "|" << std::endl;
	
	std::string		str = (line.begin()+1)->data();
	std::cout << "str = "<< str <<std::endl;
		if ((channel = find_channel(str)) == NULL)
			client->answer = ":server 403 " + str + " No such channel" + ENDLINE;
		else
		{
			std::string		mode = "itomvb";
			std::size_t		pos;
			str = (line.begin()+2)->data();
			if (check_mode(client, channel, line) == true)
			{
				if ((pos = mode.find_first_of(str[1], 0)) != std::string::npos)
				{
					std::cout << "pos find = " << pos << " corresponding to = " << str[pos] << std::endl;
					//check si client est operator et donc a le droit de faire les modes
					select_mode(client, channel, line, pos);
				}
				else
					client->answer = ":server 472 " + str + " :is unknown mode char to me for " + channel->get_name() + ENDLINE;
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
	
	std::cout << "Size mode = " << line.size() << std::endl;
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
							client->answer += ":server 441 " + client->get_nickname() + " " + channel->get_name() + " :They aren't on that channel" + ENDLINE;
					}
					else if (line.at(2).at(1) == 'o' || line.at(2).at(1) == 'v')
						client->answer += ":server 461 " + channel->get_name() + " :Not enough parameters" + ENDLINE;
					else
						return (true);
				}
				else
					client->answer += ":server 472 " + line.at(2) + " :is unknown mode char to me for " + channel->get_name() + ENDLINE;
			}
			else
				client->answer += ":server 482 " + channel->get_name() + " :You're not channel operator" + ENDLINE;
		}
		else
		{
			std::string	msg;
			msg += "+";
			if (channel->is_invite_only())
				msg += "i";
			if (channel->is_moderated())
				msg += "m";
			if (channel->is_mode_topic())
				msg += "t";
			client->answer += ":server 324 " + client->get_nickname() + " " + channel->get_name() + " " + msg + ENDLINE;
		}
	}
	else
		client->answer += ":server 441 " + client->get_nickname() + " " + channel->get_name() + " :They aren't on that channel" + ENDLINE;
	return (false);
}

void	ExecutionManager::mode_invite(Client *client, Channel *channel, std::vector<std::string> line)
{
	std::cout << "Mode Invite" << std::endl;
    if (line.at(2).at(0) == '+')
	{
        channel->change_invite_only(true);
		send_msg_to_all_clients_of_channel(":" + client->get_nickname() +" MODE " + channel->get_name() + " +i" + ENDLINE, client, channel);
	}
    else if (line.at(2).at(0) == '-')
	{
        channel->change_invite_only(false);
		send_msg_to_all_clients_of_channel(":" + client->get_nickname() +" MODE " + channel->get_name() + " -i" + ENDLINE, client, channel);
	}
}

void	ExecutionManager::mode_topic(Client *client, Channel *channel, std::vector<std::string> line)
{
	std::cout << "Mode Topic" << std::endl;
    if (line.at(2).at(0) == '+')
	{
        channel->change_mode_topic(true);
		send_msg_to_all_clients_of_channel(":" + client->get_nickname() + " MODE " + channel->get_name() + " +t" + ENDLINE, client, channel);
	}
    else if (line.at(2).at(0) == '-')
	{
        channel->change_mode_topic(false);
		send_msg_to_all_clients_of_channel(":" + client->get_nickname() + " MODE " + channel->get_name() + " -t" + ENDLINE, client, channel);
	}
}

void	ExecutionManager::mode_moderated(Client *client, Channel *channel, std::vector<std::string> line)
{
	std::cout << "Mode Moderated" << std::endl;
    if (line.at(2).at(0) == '+')
	{
        channel->change_moderated(true);
		send_msg_to_all_clients_of_channel(":" + client->get_nickname() +" MODE " + channel->get_name() + " +m" + ENDLINE, client, channel);
	}
    else if (line.at(2).at(0) == '-')
	{
        channel->change_moderated(false);
		send_msg_to_all_clients_of_channel(":" + client->get_nickname() +" MODE " + channel->get_name() + " -m" + ENDLINE, client, channel);
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
		send_msg_to_all_clients_of_channel(":" + client->get_nickname() +" MODE " + channel->get_name() + " +o " + other_client->get_nickname() + ENDLINE, client, channel);
    }
    else if (line.at(2).at(0) == '-')
    {
		other_client = find_client(line.at(3).data());
		channel->remove_operator(other_client);
		send_msg_to_all_clients_of_channel(":" + client->get_nickname() +" MODE " + channel->get_name() + " +o " + other_client->get_nickname() + ENDLINE, client, channel);
	}
	send_list_name_channel(client, channel);
	send_list_name_channel(other_client, channel);
}

void	ExecutionManager::mode_voice(Client *client, Channel *channel, std::vector<std::string> line)
{
	std::cout << "Mode Voice" << std::endl;
	Client  *other_client;

    if (line.at(2).at(0) == '+')
    {
        other_client = find_client(line.at(3).data());
        channel->add_voice_ok(other_client);
		client->answer += ":" + client->get_nickname() +" MODE " + channel->get_name() + " +v " + other_client->get_nickname() + ENDLINE;
		other_client->answer += ":" + client->get_nickname() +" MODE " + channel->get_name() + " +v " + other_client->get_nickname() + ENDLINE;
    }
    else if (line.at(2).at(0) == '-')
    {
		other_client = find_client(line.at(3).data());
		channel->remove_voice_ok(other_client);
		client->answer += ":" + client->get_nickname() +" MODE " + channel->get_name() + " -v " + other_client->get_nickname() + ENDLINE;
		other_client->answer += ":" + client->get_nickname() +" MODE " + channel->get_name() + " -v " + other_client->get_nickname() + ENDLINE;
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
			client->answer += ":" + client->get_nickname() +" MODE " + channel->get_name() + " +b " + other_client->get_nickname() + ENDLINE;
		}
		else if (line.at(2).at(0) == '-')
		{
			other_client = find_client(line.at(3).data());
			channel->remove_banned(other_client);
			client->answer += ":" + client->get_nickname() +" MODE " + channel->get_name() + " -b " + other_client->get_nickname() + ENDLINE;
		}
	}
	else if (line.size() == 3)
	{
		std::string	msg;
		msg = channel->list_banned(client->get_nickname());
		client->answer += ":server " + msg + "368 " + client->get_nickname() + " " + channel->get_name() + " :End of channel ban list" + ENDLINE;
	}
	else
		client->answer += ":server 461 " + line.at(0) + " :Not enough parameters" + ENDLINE;
}
