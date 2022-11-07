#include "../include/Server.hpp"

void	ExecutionManager::command_cap(std::vector<std::string> out)
{
	std::cout << "command CAP" << std::endl;
}

void	ExecutionManager::command_ping(std::vector<std::string> out, Client *client)
{
	std::cout << "command PING" << std::endl;
	std::string		token;

	if (out.size() != 2)
	{
		std::cerr << "NTM avec tes erreurs" << std::endl;
		//ERRROR
	}
	else 
		client->answer = "PONG server " + out.at(1);
}

void	ExecutionManager::command_nick(std::vector<std::string> out, Client *client)
{
	std::string		old_nick;

	std::cout << "command Nick" << std::endl;
	old_nick = client->get_nickname();
	if (check_nickname(out[1]))
	{
		client->_nick = 1;
		client->set_nickname(out[1]);
		if (client->wlcm_send == true)
		{
			client->answer = ":" + old_nick + "!" + client->get_name() + "@server NICK " + client->get_nickname() + ENDLINE;
			for (int i = 0; i < _clients->size(); i++)
				if(client->get_name() != _clients->at(i).get_name())
				_clients->at(i).answer = ":" + old_nick +"!" + client->get_name() + "@server NICK " + client->get_nickname() + ENDLINE;
		}
		else if (client->get_name().size() != 0 && !client->wlcm_send)
		{
			client->answer = ":server 001 " + client->get_nickname() + " :Welcome to the Internet Relay Network " + client->get_nickname() + "!" + client->get_name() + "@" + _address + ENDLINE;
			client->wlcm_send = 1;
		}
	}
	else
	{
		client->answer = ":server 433 " + out[1] + ":Nickname is already in use" + ENDLINE;
	}
	//ajouter si quelqu'un a deja le nickname
}

void	ExecutionManager::send_msg_to_client(std::string msg, Client *other_client)
{
	other_client->answer = msg;
	std::cout << other_client->answer << std::endl;
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
	if (client->_user)
	{
		// ERROR REGISTRATION
		return;
	}
	client->_user = 1;
	std::cout << "command Client" << std::endl;
	client->set_name(out[1]);
	if (client->get_nickname().size() != 0 && !client->wlcm_send)
	{
		client->answer = ":server 001 " + client->get_nickname() + " :Welcome to the Internet Relay Network " + client->get_nickname() + "!" + client->get_name() + "@" + _address + ENDLINE;
		client->wlcm_send = 1;
	}
}

void	ExecutionManager::command_quit(Client *client, int index)
{
	std::cout << "command Quit" << std::endl;
	deleteClient(index);
}
