#include "../include/Server.hpp"

void	ExecutionManager::command_cap(std::vector<std::string> out)
{
	std::cout << "command CAP" << std::endl;
}

void	ExecutionManager::command_ping(std::vector<std::string> out)
{
	std::cout << "command PING" << std::endl;
}

void	ExecutionManager::command_nick(std::vector<std::string> out, User *user)
{
	std::string		old_nick;
	std::cout << "command Nick" << std::endl;
	old_nick = user->get_nickname();
	if (check_nickname(out[1]))
	{
		user->set_nickname(out[1]);
		if (user->wlcm_send == true)
			user->answer = ":" + old_nick + "!" + user->get_name() + "@server NICK " + user->get_nickname() + ENDLINE;
		else if (user->get_name().size() != 0)
		{
			user->answer = ":server 001 " + user->get_nickname() + " :Welcome to the Internet Relay Network " + user->get_nickname() + "!" + user->get_name() + "@" + _address + ENDLINE;
			user->wlcm_send = 1;
		}
	}
	else
	{
		user->answer = ":server 433 " + out[1] + ":Nickname is already in use" + ENDLINE;
	}
	//ajouter si quelqu'un a deja le nickname
}

bool	ExecutionManager::check_nickname(std::string nickname)
{
	for (std::vector<User>::iterator it = _users->begin(); it != _users->end(); it++)
		if (it->get_nickname() == nickname)
			return (false);
	return (true);
}

void	ExecutionManager::command_user(std::vector<std::string> out, User *user)
{
	std::cout << "command User" << std::endl;
	user->set_name(out[1]);
	if (user->get_nickname().size() != 0 && !user->wlcm_send)
	{
		user->answer = ":server 001 " + user->get_nickname() + " :Welcome to the Internet Relay Network " + user->get_nickname() + "!" + user->get_name() + "@" + _address + ENDLINE;
		user->wlcm_send = 1;
	}
}

void	ExecutionManager::command_quit(std::vector<std::string> out, User *user)
{
	std::cout << "command Quit" << std::endl;
}