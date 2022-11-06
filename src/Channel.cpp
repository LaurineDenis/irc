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
	std::cout << "Constructor Channel by default called" << std::endl;
	_operator = creator;
	_name = name;
	_clients = new std::vector<Client>;
	_clients->push_back(*creator);
	//mettre le client en operator dans client
	_banned = new std::vector<Client>;
	_topic = "";
	_topic_client = "";
	_topic_time = "";
}

Channel::Channel(Channel const &cpy)
{
	std::cout << "Constructor Channel by copy " << cpy._name <<  " called" << std::endl;
	*this = cpy;
}

Channel		&Channel::operator=(Channel const &cpy)
{
	std::cout << "Channel operator = called" << std::endl;
	_operator = cpy._operator;
	_name = cpy._name;
	_clients = new std::vector<Client>;
	_banned = new std::vector<Client>;
	_clients = cpy._clients;
	_banned = cpy._banned;
	_topic = cpy._topic;
	_topic_client = cpy._topic_client;
	_topic_time = cpy._topic_time;
	return (*this);
}

Channel::~Channel(void)
{
	std::cout << "Destructor Channel " << _name << " called" << std::endl;
}

std::string				Channel::get_name()
{
	return (_name);
}

void					Channel::set_name(std::string name)
{
	_name = name;
}

std::string				Channel::get_topic()
{
	return (_topic);
}

void					Channel::set_topic(std::string topic)
{
	_topic = topic;
}

std::string				Channel::get_topic_client()
{
	return (_topic_client);
}

void					Channel::set_topic_client(std::string topic_client)
{
	_topic_client = topic_client;
}

std::string				Channel::get_topic_time()
{
	return (_topic_time);
}

void					Channel::set_topic_time(std::string topic_time)
{
	_topic_time = topic_time;
}

Client				*Channel::get_operator()
{
	return (_operator);
}

void					Channel::set_operator(Client *client)
{
	_operator = client;
}

bool				Channel::banned_client(Client *client)
{
	_banned->push_back(*client);
}

bool				Channel::is_banned(Client *client)
{
	for (std::vector<Client>::iterator ite = _banned->begin(); ite != _banned->end(); ite++)
		if (ite->get_nickname() == client->get_nickname())
			return (true) ;
	return (false);
}
