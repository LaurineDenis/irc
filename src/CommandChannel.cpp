#include "../include/Irc.hpp"

void	ExecutionManager::change_topic(std::string topic, std::string client, Channel *channel)
{
	// char			time_test[300];
	time_t	timestamp = time( NULL );
    // struct tm * pTime = localtime( & timestamp );
	// strftime(time_test, 300, "%a, %d %h %G %T", pTime);
	// std::string		time_string;
	channel->set_topic(topic);
	channel->set_topic_client(client);
	channel->set_topic_time(ctime(&timestamp));
}

void	ExecutionManager::command_topic(std::vector<std::string> out, Client *client)
{
	std::cout << "command TOPIC" << std::endl;
	Channel		*channel;
	std::string	channel_name;

	if (out[1][0] == '#')
	{
		channel_name = out[1].erase(0, 1);
		if ((channel = find_channel(channel_name)) == NULL)
			client->answer = ":server 403 " + out[1] + " No such channel" + ENDLINE;
		else
		{
			std::cout << "Topic Time == " << channel->get_topic_time() << std::endl;
			if (out.size() <= 2)
			{
				if (channel->get_topic() == "")
					client->answer = ":server 331 " +  client->get_name() + " #" + channel->get_name() + " :No topic is set" + ENDLINE;
				else
					client->answer = ":server 332 " +  client->get_name() + " #" + channel->get_name() + " :" + channel->get_topic() + ENDLINE + ":server 333 " +  client->get_name() + " #" + channel->get_name() + " " + channel->get_topic_client() + " " + channel->get_topic_time() + ENDLINE;
			}
			else if (out[2][0] != ':')
				client->answer = ":server 461 " +  client->get_name() + " TOPIC :Not enough parameters" + ENDLINE;
			else
			{
				change_topic(out[2].erase(0, 1), client->get_name(), channel);
				client->answer = ":server 332 " +  client->get_name() + " #" + channel->get_name() + " :" + channel->get_topic() + ENDLINE;
			}
		}
	}
	else
	{
		client->answer = ":server 403 " + out[1] + " No such channel" + ENDLINE;
	}
}

bool	ExecutionManager::parse_channel_name(std::string channel_name)
{
	if (channel_name[0] == '#')
	{
		return (true);
	}
	else
		return (false);
}

void	ExecutionManager::send_topic_reply(Client *client, Channel *channel)
{
	std::string	msg;

	client->answer += ":" + client->get_nickname() + "!" + client->get_nickname() + "@server JOIN " + channel->get_name() + ENDLINE;
	if (channel->get_topic() != "")
		client->answer += ":server 332 " +  client->get_name() + " #" + channel->get_name() + " :" + channel->get_topic() + ENDLINE + ":server 333 " +  client->get_name() + " " + channel->get_name() + " " + channel->get_topic_client() + " " + channel->get_topic_time() + ENDLINE;
	if (channel->_clients->size() > 1)
	{
		for (int i = 0; i < channel->_clients->size(); i++)
		{
			if (i != 0)
				msg += " ";
			if (channel->is_operator(&_clients->at(i)))
				msg+= "@";
			msg += channel->_clients->at(i).get_nickname();
		}
		client->answer += ":server 353 " +  client->get_name() + " = " + channel->get_name() + " :" + msg + ENDLINE;	
	}
}

void	ExecutionManager::command_join(std::vector<std::string> line, Client *client)
{
	Channel				*channel;
	std::string			msg;

	//Affichage
	for (std::vector<std::string>::iterator it = line.begin(); it != line.end(); ++it)
		std::cout << "|" << *it << "|" << std::endl;
	for (std::vector<std::string>::iterator it = line.begin()+1; it != line.end(); ++it)
	{
		std::string		str = it->data();
		if (parse_channel_name(str))
		{
			if ((channel = find_channel(str.erase(0, 1))) == NULL)
			{
				//creer le channel si il existe pas correspondant puis add channel in server
				channel = new Channel(client, it->data());
				_channels->push_back(*channel);
			}
			add_channel_in_client(channel, client);
			send_topic_reply(client, channel);
			send_msg_to_channel_clients(":" + client->get_nickname() + "!" + client->get_nickname() + "@server JOIN " + channel->get_name() + ENDLINE, client, channel);
		}
		else
			client->answer = ":server 403 " + str + " No such channel" + ENDLINE;
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
	std::string	channel_name;

	if (out[1][0] == '#')
	{
		channel_name = out[1].erase(0, 1);
		if ((channel = find_channel(channel_name)) == NULL)
		{
			//Le channel n'existe pas => Pas de message d'erreurs ?
			std::cout << "No Channel found" << std::endl;
			client->answer = ":server 403 " + out[1] + " No such channel" + ENDLINE;
		}
		else
		{
			//quit le channel qui existe deja
			remove_client_of_channel(channel, client);
			client->answer = ":" + client->get_nickname() + "!" + client->get_name() + "@server PART #" + channel->get_name() + ENDLINE;
			std::cout << "send = " << client->answer << std::endl;
			send_msg_to_channel_clients(":" + client->get_nickname() + "!" + client->get_nickname() + "@server PART #" + channel->get_name() + ENDLINE, client, channel);
		}
	}
	else
	{
		client->answer = ":server 403 " + out[1] + " No such channel" + ENDLINE;
	}
}

void	ExecutionManager::command_kick(std::vector<std::string> out, Client *client)
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
			client->answer = ":server 403 " + out[1] + " No such channel" + ENDLINE;
		}
		else
		{
			if (channel->is_operator(client) == true)
			{
				Client	*clientToBan;
				clientToBan = find_client(out[2]);
				// remove client from channel
				remove_client_of_channel(channel, clientToBan);
				// add client in banned list
				channel->change_banned(clientToBan);
				// send msg to client banned
				client->answer = ":" + client->get_nickname() + "!" + client->get_name() + "@server KICK #" + channel->get_name() + " " + out[2] + ENDLINE;
				// send msg to client who banned
				send_msg_to_client(client->answer, clientToBan);
			}
			else
			{
				client->answer = ":server 482 " + client->get_nickname() + " #" + out[1] + " :You're not channel operator" + ENDLINE;
			}
		}
	}
	else
	{
		client->answer = ":server 403 " + out[1] + " No such channel" + ENDLINE;
	}
}
