#include "../include/Server.hpp"

void	ExecutionManager::command_cap(std::vector<std::string> out)
{
	(void)out;
}

void	ExecutionManager::command_ping(std::vector<std::string> out, Client *client)
{
	std::string		token;

	if (out.size() != 2)
		client->answer += ERR_NEEDMOREPARAMS(out.at(0));
	else 
		client->answer += "PONG server " + out.at(1) + ENDLINE;
}

void	ExecutionManager::command_nick(std::vector<std::string> out, Client *client)
{
	std::string		old_nick;

	old_nick = client->get_nickname();
	if (check_nickname(out[1]))
	{
		client->_nick = 1;
		client->set_nickname(out[1]);
		if (client->wlcm_send == true)
		{
			client->answer += MSG_NICK(old_nick, client->get_nickname());
			for (unsigned long i = 0; i < _clients->size(); i++)
				if(client->get_name() != _clients->at(i).get_name())
					_clients->at(i).answer = MSG_NICK(old_nick, client->get_nickname());
		}
		else if (client->get_name().size() != 0 && !client->wlcm_send)
		{
			if (client->_pw)
			{
				client->answer += RPL_WELCOME(client->get_nickname());
				client->wlcm_send = 1;
			}
			else
			{
				client->_del = 1;
				client->answer += ERR_PASSWDMISMATCH(client->get_nickname());
			}
		}
	}
	else
		client->answer += ERR_NICKNAMEINUSE(out[1]);
}

void	ExecutionManager::send_msg_to_client(std::string msg, Client *client)
{
	client->answer += msg;
}

Client	*ExecutionManager::find_client(std::string nickname)
{
	size_t		size;

	if (!_clients)
		return (NULL);
	size = _clients->size();
	for (size_t i = 0; i < size; i++)
	{
		if (nickname == _clients->at(i).get_nickname())
			return (&_clients->at(i));
	}
	return (NULL);
}

bool	ExecutionManager::check_nickname(std::string nickname)
{
	for (std::vector<Client>::iterator it = _clients->begin(); it != _clients->end(); it++)
		if (it->get_nickname() == nickname)
			return (false);
	return (true);
}

void	ExecutionManager::command_client(std::vector<std::string> out, Client *client)
{
	client->_user = 1;
	client->set_name(out[1]);
	if (client->get_nickname().size() != 0 && !client->wlcm_send)
	{
		if (client->_pw)
		{
			client->answer = RPL_WELCOME(client->get_nickname());
			client->wlcm_send = 1;
		}
		else 
		{
			client->answer = ERR_PASSWDMISMATCH(client->get_nickname());
			client->_del = 1;
		}
	}
	else 
		client->answer += ERR_ALREADYREGISTRED;
}

void	ExecutionManager::command_quit(Client *client, int index)
{
	std::cout << "command Quit" << std::endl;
	client->_del = 1;
	(void)index;
}
