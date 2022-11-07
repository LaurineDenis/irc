#include "../include/Irc.hpp"

Channel::Channel(void)
{
	std::cout << "Constructor Channel by default called" << std::endl;
	_topic = "";
	_topic_client = "";
	_topic_time = "";
}

Channel::Channel(Client *creator, std::string name)
{
	std::cout << "Constructor Channel by name called" << std::endl;
	_creator = creator;
	_name = name;
	_clients = new std::vector<Client>;
	_operator = new std::vector<Client>;
	_invited = new std::vector<Client>;
	_banned = new std::vector<Client>;
	_voice_ok = new std::vector<Client>;
	_clients->push_back(*creator);
	//mettre le client en operator dans le channel
	_operator->push_back(*creator);
	_topic = "";
	_topic_client = "";
	_topic_time = "";
	_invite_only = false;
	_mode_topic = false;
	_moderated = false;
}

Channel::Channel(Channel const &cpy)
{
	std::cout << "Constructor Channel by copy " << cpy._name <<  " called" << std::endl;
	*this = cpy;
}

Channel		&Channel::operator=(Channel const &cpy)
{
	std::cout << "Channel operator = called" << std::endl;
	_clients = new std::vector<Client>;
	_operator = new std::vector<Client>;
	_banned = new std::vector<Client>;
	_invited = new std::vector<Client>;
	_voice_ok = new std::vector<Client>;
	_creator = cpy._creator;
	_operator = cpy._operator;
	_banned = cpy._banned;
	_invited = cpy._invited;
	_voice_ok = cpy._voice_ok;
	_clients = cpy._clients;
	_name = cpy._name;
	_topic = cpy._topic;
	_topic_client = cpy._topic_client;
	_topic_time = cpy._topic_time;
	_invite_only = cpy._invite_only;
	_mode_topic = cpy._mode_topic;
	_moderated = cpy._moderated;
	return (*this);
}

Channel::~Channel(void)
{
	std::cout << "Destructor Channel " << _name << " called" << std::endl;
}

std::string		Channel::get_name()
{
	return (_name);
}

void			Channel::set_name(std::string name)
{
	_name = name;
}

std::string		Channel::get_topic()
{
	return (_topic);
}

void			Channel::set_topic(std::string topic)
{
	_topic = topic;
}

std::string				Channel::get_topic_client()
{
	return (_topic_client);
}

void			Channel::set_topic_client(std::string topic_client)
{
	_topic_client = topic_client;
}

std::string		Channel::get_topic_time()
{
	return (_topic_time);
}

void			Channel::set_topic_time(std::string topic_time)
{
	_topic_time = topic_time;
}

Client			*Channel::get_creator()
{
	return (_creator);
}

void			Channel::set_creator(Client *client)
{
	_creator = client;
}

bool			Channel::add_banned(Client *client)
{
	if (is_banned(client) == true)
		return (false);
	_banned->push_back(*client);
	return (true);
}

bool			Channel::remove_banned(Client *client)
{
	if (is_banned(client) == false)
		return (false);
	for (std::vector<Client>::iterator it = _banned->begin(); it != _banned->end(); ++it)
	{
		if (it->get_nickname() == client->get_nickname())
		{
			_banned->erase(it);
			return (true);
		}
	}
	return (false);
}

bool			Channel::is_banned(Client *client)
{
	for (std::vector<Client>::iterator ite = _banned->begin(); ite != _banned->end(); ite++)
	{
		std::cout << "Banned = " << ite->get_nickname() << std::endl;
		if (ite->get_nickname() == client->get_nickname())
			return (true) ;
	}
	return (false);
}

bool			Channel::add_operator(Client *client)
{
	if (is_operator(client) == true)
		return (false);
	_operator->push_back(*client);
	return (true);
}

bool			Channel::remove_operator(Client *client)
{
	if (is_operator(client) == false)
		return (false);
	for (std::vector<Client>::iterator it = _operator->begin(); it != _operator->end(); ++it)
	{
		if (it->get_nickname() == client->get_nickname())
		{
			_operator->erase(it);
			return (true);
		}
	}
	return (false);
}

bool			Channel::is_operator(Client *client)
{
	for (std::vector<Client>::iterator ite = _operator->begin(); ite != _operator->end(); ite++)
		if (ite->get_nickname() == client->get_nickname())
			return (true) ;
	return (false);
}

bool			Channel::add_invited(Client *client)
{
	if (is_invited(client) == true)
		return (false);
	_invited->push_back(*client);
	return (true);
}

bool			Channel::remove_invited(Client *client)
{
	if (is_invited(client) == false)
		return (false);
	for (std::vector<Client>::iterator it = _invited->begin(); it != _invited->end(); ++it)
	{
		if (it->get_nickname() == client->get_nickname())
		{
			_invited->erase(it);
			return (true);
		}
	}
	return (false);
}

bool			Channel::is_invited(Client *client)
{
	for (std::vector<Client>::iterator ite = _invited->begin(); ite != _invited->end(); ite++)
		if (ite->get_nickname() == client->get_nickname())
			return (true) ;
	return (false);
}

bool			Channel::add_voice_ok(Client *client)
{
	if (is_voice_ok(client) == true)
		return (false);
	_voice_ok->push_back(*client);
	return (true);
}

bool			Channel::remove_voice_ok(Client *client)
{
	if (is_voice_ok(client) == false)
		return (false);
	for (std::vector<Client>::iterator it = _voice_ok->begin(); it != _voice_ok->end(); ++it)
	{
		if (it->get_nickname() == client->get_nickname())
		{
			_voice_ok->erase(it);
			return (true);
		}
	}
	return (false);
}

bool			Channel::is_voice_ok(Client *client)
{
	for (std::vector<Client>::iterator ite = _voice_ok->begin(); ite != _voice_ok->end(); ite++)
		if (ite->get_nickname() == client->get_nickname())
			return (true) ;
	return (false);
}

bool			Channel::is_invite_only()
{
	if (_invite_only)
		return (true);
	return (false);
}

bool			Channel::is_mode_topic()
{
	if (_mode_topic)
		return (true);
	return (false);
}

bool			Channel::is_moderated()
{
	if (_moderated)
		return (true);
	return (false);
}

void			Channel::change_invite_only(bool invite_only)
{
	_invite_only = invite_only;
}

void			Channel::change_mode_topic(bool mode_topic)
{
	_mode_topic = mode_topic;
}

void			Channel::change_moderated(bool moderated)
{
	_moderated = moderated;
}

std::string		Channel::list_banned(std::string name)
{
	std::string	msg;

	for (std::vector<Client>::iterator is_ban = _banned->begin(); is_ban < _banned->end(); is_ban++)
	{
		msg += "367 " + name + " " + get_name() + " " + is_ban->get_nickname() + " " + ENDLINE;
	}
	return (msg);
}
