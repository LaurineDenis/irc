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
			if ((pos = mode.find_first_of(str[1], 0)) != std::string::npos)
			{
				std::cout << "pos find = " << pos << " corresponding to = " << str[pos] << std::endl;
				//check si client est operator et donc a le droit de faire les modes
				if (channel->is_operator(client) == true)
				{
					select_mode(client, channel, line, pos);
				}
			}
			else
				client->answer = ":server 472 " + str.erase(0, 1) + " :is unknown mode char to me for <channel>" + ENDLINE;
		}
}

void	ExecutionManager::select_mode(Client *client, Channel *channel, std::vector<std::string> line, std::size_t pos)
{
	void		(ExecutionManager::*function[])
	(Client *client, Channel *channel, std::vector<std::string> line) = 
	{&ExecutionManager::mode_invite, &ExecutionManager::mode_topic, &ExecutionManager::mode_operator, &ExecutionManager::mode_moderated, &ExecutionManager::mode_voice, &ExecutionManager::mode_banned};

	(this->*function[pos])(client, channel, line);
}

void	ExecutionManager::mode_invite(Client *client, Channel *channel, std::vector<std::string> line)
{
	std::cout << "Mode Invite" << std::endl;
    if (line.at(2).at(0) == '+')
        channel->change_invite_only(true);
    else if (line.at(2).at(0) == '-')
        channel->change_invite_only(false);
    else
        std::cout << "Erreur parsing Mode" <<std::endl;
	client->answer = ":server 324 " + channel->get_name() + " +i" + ENDLINE;
}

void	ExecutionManager::mode_topic(Client *client, Channel *channel, std::vector<std::string> line)
{
	std::cout << "Mode Topic" << std::endl;
    if (line.at(2).at(0) == '+')
        channel->change_mode_topic(true);
    else if (line.at(2).at(0) == '-')
        channel->change_mode_topic(false);
    else
        std::cout << "Erreur parsing Mode" <<std::endl;
	client->answer = ":server 324 " + channel->get_name() + " +t" + ENDLINE;
}

void	ExecutionManager::mode_moderated(Client *client, Channel *channel, std::vector<std::string> line)
{
	std::cout << "Mode Moderated" << std::endl;
    if (line.at(2).at(0) == '+')
        channel->change_moderated(true);
    else if (line.at(2).at(0) == '-')
        channel->change_moderated(false);
    else
        std::cout << "Erreur parsing Mode" <<std::endl;
	client->answer = ":server 324 " + channel->get_name() + " +m" + ENDLINE;
}

void	ExecutionManager::check_mode(Client *client, Channel *channel, std::vector<std::string> line)
{
	if (line.at(0).at(1) == '+' || line.at(0).at(1) == '-')
	{
	}
}

void	ExecutionManager::mode_operator(Client *client, Channel *channel, std::vector<std::string> line)
{
	std::cout << "Mode Operator" << std::endl;
    Client  *other_client;

    if (line.at(2).at(0) == '+')
    {
        if ((other_client = find_client(line.at(3).data())) != NULL)
		{
			if (is_in_channel(channel, client) == true)
			{
	            channel->add_operator(other_client);
				client->answer += ":server 324 " + channel->get_name() + " +o " + other_client->get_nickname() + ENDLINE;
			}
			else
				client->answer += ":server 441 " + client->get_nickname() + " " + channel->get_name() + " :They aren't on that channel" + ENDLINE;
		}
		else
			std::cout << "This client doesn't exist" << std::endl;
    }
    else if (line.at(2).at(0) == '-')
    {
		if ((other_client = find_client(line.at(3).data())) != NULL)
		{
			if (is_in_channel(channel, client) == true)
			{
				if (channel->is_operator(client) == true)
	            {
					channel->remove_operator(other_client);
					client->answer += ":server 324 " + channel->get_name() + " -o " + other_client->get_nickname() + ENDLINE;
				}
				else
					std::cout << "This client is not an operator" << std::endl;
			}
			else
				client->answer += ":server 441 " + client->get_nickname() + " " + channel->get_name() + " :They aren't on that channel" + ENDLINE;
		}
		else
			std::cout << "This client doesn't exist" << std::endl;
	}
    else
        std::cout << "Erreur parsing Mode" <<std::endl;
}

void	ExecutionManager::mode_voice(Client *client, Channel *channel, std::vector<std::string> line)
{
	std::cout << "Mode Voice" << std::endl;
	Client  *other_client;

    if (line.at(2).at(0) == '+')
    {
        if ((other_client = find_client(line.at(3).data())) != NULL)
		{
			if (is_in_channel(channel, client) == true)
	            channel->add_voice_ok(other_client);
			else
				client->answer += ":server 441 " + client->get_nickname() + " " + channel->get_name() + " :They aren't on that channel" + ENDLINE;
		}
		else
			std::cout << "This client doesn't exist" << std::endl;
    }
    else if (line.at(2).at(0) == '-')
    {
		if ((other_client = find_client(line.at(3).data())) != NULL)
		{
			if (is_in_channel(channel, client) == true)
			{
				if (channel->is_voice_ok(client) == true)
	            	channel->remove_voice_ok(other_client);
				else
					std::cout << "This client has not voice ok" << std::endl;
			}
			else
				client->answer += ":server 441 " + client->get_nickname() + " " + channel->get_name() + " :They aren't on that channel" + ENDLINE;
		}
		else
			std::cout << "This client doesn't exist" << std::endl;
	}
    else
        std::cout << "Erreur parsing Mode" <<std::endl;
}

void	ExecutionManager::mode_banned(Client *client, Channel *channel, std::vector<std::string> line)
{
	std::cout << "Mode Banned" << std::endl;
	Client  *other_client;
  
	if (line.size() > 3)
	{
		if (line.at(2).at(0) == '+')
		{
			if ((other_client = find_client(line.at(3).data())) != NULL)
			{
				if (is_in_channel(channel, client) == true)
				{
					channel->add_banned(other_client);
					client->answer += ":server 324 " + channel->get_name() + " +b " + other_client->get_nickname() + ENDLINE;
				}
				else
					client->answer += ":server 441 " + client->get_nickname() + " " + channel->get_name() + " :They aren't on that channel" + ENDLINE;
			}
			else
				std::cout << "This client doesn't exist" << std::endl;
		}
		else if (line.at(2).at(0) == '-')
		{
			if ((other_client = find_client(line.at(3).data())) != NULL)
			{
				if (is_in_channel(channel, client) == true)
				{
					if (channel->is_banned(other_client) == true)
					{
						channel->remove_banned(other_client);
						client->answer += ":server 324 " + channel->get_name() + " -b " + other_client->get_nickname() + ENDLINE;
					}
					else
						std::cout << "This client is not banned" << std::endl;
				}
				else
					client->answer += ":server 441 " + client->get_nickname() + " " + channel->get_name() + " :They aren't on that channel" + ENDLINE;
			}
			else
				std::cout << "This client doesn't exist" << std::endl;
		}
		else
			std::cout << "Erreur parsing Mode" <<std::endl;
	}
	else if (line.size() == 2)
	{
		//list banned people
	}
	else
		client->answer += ":server 461 " + line.at(0) + " :Not enough parameters" + ENDLINE;
}
