#include "../include/Irc.hpp"
#include <ctime>
#include <string>

void	ExecutionManager::change_topic(std::string topic, std::string client, Channel *channel)
{
	time_t	rawtime;

	time(&rawtime);
	channel->set_topic(topic);
	channel->set_topic_client(client);
	channel->set_topic_time(std::to_string(rawtime));
}

void	ExecutionManager::command_topic(std::vector<std::string> out, Client *client)
{
	std::cout << "command TOPIC" << std::endl;
	Channel		*channel;
	std::string	channel_name;

	if (out[1][0] == '#')
	{
		channel_name = out[1];
		if ((channel = find_channel(channel_name)) == NULL)
			client->answer += ERR_NOSUCHCHANNEL(out[1]);
		else
		{
			if (is_in_channel(channel, client))
			{
				if (out.size() <= 2)
				{
					if (channel->get_topic() == "")
						client->answer += RPL_NOTOPIC(client->get_nickname(), channel->get_name());
					else
						client->answer += RPL_TOPIC(client->get_nickname(), channel->get_name(), channel->get_topic()) + RPL_TOPICWHOTIME(client->get_nickname(), channel->get_name(), channel->get_topic_client(), channel->get_topic_time());
				}
				else if (out[2][0] != ':')
					client->answer += ERR_NEEDMOREPARAMS(out.at(0));
				else
				{
					if (channel->is_mode_topic() && channel->is_operator(client))
					{
						change_topic(out[2].erase(0, 1), client->get_nickname(), channel);
						send_msg_to_all_clients_of_channel(RPL_TOPIC(client->get_nickname(), channel->get_name(), channel->get_topic()), client, channel);
					}
					else
						client->answer += ERR_CHANOPRIVSNEEDED(channel->get_name());
				}
			}
			else
				client->answer += ERR_NOTONCHANNEL(channel->get_name());
		}
	}
	else
	{
		client->answer += ERR_NOSUCHCHANNEL(out[1]);
	}
}

std::vector<std::string>	ExecutionManager::parse_channel_name(std::vector<std::string> channel_name, Client *client)
{
	std::vector<std::string>	names;
	std::string					s;

	channel_name.resize(2);
	names = split(channel_name.at(1), ",");
	for (unsigned long i = 0; i < channel_name.size(); i++)
		std::cout << "|||" << channel_name.at(i) << std::endl;
	for (unsigned long i = 0; i < names.size(); i++)
	{
		s = names.at(i);
		if (s[0] != '#')
		{
			client->answer += ERR_NOSUCHCHANNEL(names.at(i));
			names.erase(names.begin() + i);
			// ERROR not a channel name
		}
		else 
		{
			s.erase(0, 1);
			if (s.find_first_of(" \7") != std::string::npos)
				names.erase(names.begin() + i);

		}
	}
	return names;
}

void	ExecutionManager::send_topic_reply(Client *client, Channel *channel)
{
	client->answer += MSG_JOIN(channel->get_name(), client->get_nickname());
	if (channel->get_topic() != "")
		client->answer += RPL_TOPIC(client->get_nickname(), channel->get_name(), channel->get_topic());
	send_list_name_channel(client, channel);
}

void	ExecutionManager::send_list_name_channel(Client *client, Channel *channel)
{
	std::string	lst_names;

	if (channel->_clients->size() >= 1)
	{
		for (unsigned long i = 0; i < channel->_clients->size(); i++)
		{
			if (i != 0)
				lst_names += " ";
			if (channel->is_operator(&channel->_clients->at(i)) == true)
				lst_names+= "@";
			lst_names += channel->_clients->at(i).get_nickname();
		}
		client->answer += RPL_NAMREPLY(channel->get_name(), client->get_nickname(), lst_names);	
	}
}

bool	ExecutionManager::check_right_channel(std::string channel_name, Client *client)
{
	Channel	*channel;

	if ((channel = find_channel(channel_name)) != NULL)
	{
		if (is_in_channel(channel, client))
		{
			client->answer += ERR_USERONCHANNEL(client->get_nickname(), channel->get_name());
			return (false);
		}
		if ((channel->is_invite_only() && channel->is_invited(client)) || !channel->is_invite_only())
		{
			if (!channel->is_banned(client))
				return (true);
			else
				client->answer += ERR_BANNEDFROMCHAN(channel->get_name());
		}
		else
			client->answer += ERR_INVITEONLYCHAN(channel->get_name());
		return (false);
	}
	return (true);
}

void	ExecutionManager::command_join(std::vector<std::string> line, Client *client)
{
	Channel						*channel;
	std::string					msg;
	std::vector<std::string>	channel_names;
	time_t						rawtime;

	channel_names = parse_channel_name(line, client);
	for (unsigned long i = 0; i < channel_names.size(); i++)
	{
		channel = find_channel(channel_names.at(i));
		if (channel == NULL)
		{
			time(&rawtime);
			channel = new Channel(client, channel_names.at(i), std::to_string(rawtime));
			_channels->push_back(*channel);
		}
		add_channel_in_client(channel, client);
		send_topic_reply(client, channel);
    send_msg_to_channel_clients(MSG_JOIN(channel->get_name(), client->get_nickname()), client, channel);
	}
}

bool	is_in_channel(Channel *channel, Client *client)
{
	for (std::vector<Client>::iterator ite = channel->_clients->begin(); ite != channel->_clients->end(); ite++)
		if (ite->get_nickname() == client->get_nickname())
			return (true) ;
	return (false);
}

bool	add_channel_in_client(Channel *channel, Client *client)
{
	//add channel in client
	if (channel->is_banned(client) == true)
		return (false);
	client->_channels->push_back(*channel);
	client->set_nb_channel(client->get_nb_channel() + 1);
	if (is_in_channel(channel, client) == false)
		channel->_clients->push_back(*client);
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

void	ExecutionManager::remove_client_of_channel(Channel *channel, Client *client)
{
	int		i = 0;
	//supprimer le channel dans client
	for (std::vector<Channel>::iterator ite = client->_channels->begin(); ite != client->_channels->end(); ite++, i++)
		if (ite->get_name() == channel->get_name())
			break ;
	client->_channels->erase(client->_channels->begin() + i);
	//supprimer le client dans channel
	i = 0;
	for (std::vector<Client>::iterator ite = channel->_clients->begin(); ite != channel->_clients->end(); ite++, i++)
		if (ite->get_nickname() == client->get_nickname())
			break ;
	channel->_clients->erase(channel->_clients->begin() + i);
	//supprimer le channel si 0 clients
	if (channel->_clients->empty())
		delete_channel(channel);
}

void	ExecutionManager::command_part(std::vector<std::string> out, Client *client)
{
	std::cout << "command Part" << std::endl;
	Channel		*channel;
	std::vector<std::string>	channel_names;

	channel_names = parse_channel_name(out, client);
	for (unsigned long i = 0; i < channel_names.size(); i++)
	{
		if ((channel = find_channel(channel_names.at(i))) == NULL)
			client->answer += ERR_NOSUCHCHANNEL(out[1]);
		else
		{
			if (is_in_channel(channel, client))
			{
				// client->answer += ":" + client->get_nickname() + "!" + client->get_name() + "@server PART " + channel->get_name() + ENDLINE;
				std::string	part_msg;
				if (out.size() >= 3)
					part_msg = out.at(2);
				else
					part_msg = "";
				send_msg_to_channel_clients(MSG_PART(channel->get_name(), client->get_nickname(), part_msg), client, channel);
				remove_client_of_channel(channel, client);
			}
			else
				client->answer += ERR_NOTONCHANNEL(channel->get_name());
		}
	}
}

void	ExecutionManager::command_kick(std::vector<std::string> out, Client *client)
{
	std::cout << "command Kick" << std::endl;
	Channel		*channel;
	std::vector<std::string>	channel_names;
	std::vector<std::string>	user_names;

	channel_names = parse_channel_name(out, client);
	for (unsigned long i = 0; i < channel_names.size(); i++)
	{
		if ((channel = find_channel(channel_names.at(i))) == NULL)
			client->answer += ERR_NOSUCHCHANNEL(channel_names.at(i));
		else
		{
			if (is_in_channel(channel, client))
			{
				if (channel->is_operator(client) == true)
				{
					user_names = split(out.at(2), ",");
					for (unsigned long j = 0; j < user_names.size(); j++)
					{
						Client	*clientToKick;
						if ((clientToKick = find_client(user_names.at(j))) != NULL && is_in_channel(channel, clientToKick) == true)
						{
							std::string		kick_msg;
							remove_client_of_channel(channel, clientToKick);
							if (out.size() >= 4)
								kick_msg = out[3];
							else
								kick_msg = "";
							clientToKick->answer += MSG_KICK(client->get_nickname(), channel->get_name(), clientToKick->get_nickname(), kick_msg);
							client->answer += MSG_KICK(client->get_nickname(), channel->get_name(), clientToKick->get_nickname(), kick_msg);
						}
						else
							client->answer += ERR_USERNOTINCHANNEL(user_names.at(j), channel->get_name());
					}
				}
				else
					client->answer += ERR_CHANOPRIVSNEEDED(channel->get_name());
			}
			else
				client->answer += ERR_NOTONCHANNEL(channel->get_name());
		}
	}
}

void	ExecutionManager::command_invite(std::vector<std::string> line, Client *client)
{
	Channel				*channel;
	std::string			msg;
	Client				*to_invite;

	if ((to_invite = find_client(line[1])) != NULL && (channel = find_channel(line[2])) != NULL)
	{
		if (is_in_channel(channel, client) == true)
		{
			if (is_in_channel(channel, to_invite) == false)
			{
				if ((channel->is_invite_only() == true && channel->is_operator(client)) || channel->is_invite_only() != true)
				{
					if (channel->is_invite_only() == true)
						channel->add_invited(to_invite);
					client->answer += RPL_INVITING(client->get_nickname(), channel->get_name(), to_invite->get_nickname());
					to_invite->answer += RPL_INVITING(client->get_nickname(), channel->get_name(), to_invite->get_nickname());
				}
				else
					client->answer += ERR_CHANOPRIVSNEEDED(channel->get_name());
			}
			else
				client->answer += ERR_USERONCHANNEL(line[2], channel->get_name());
		}
		else
			client->answer += ERR_NOTONCHANNEL(channel->get_name());
	}
	else
		client->answer += ERR_NOSUCHNICK(line[1]);
}
