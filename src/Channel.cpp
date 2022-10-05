#include "../include/Irc.hpp"

Channel::Channel(void)
{
	std::cout << "Constructor Channel by default called" << std::endl;
	_topic = "";
	_topic_user = "";
	_topic_time = "";
}

Channel::Channel(User *creator, std::string name)
{
	std::cout << "Constructor Channel by default called" << std::endl;
	_operator = creator;
	_name = name;
	_users = new std::vector<User>;
	_users->push_back(*creator);
	//mettre le user en operator dans user
	_banned = new std::vector<User>;
	_topic = "";
	_topic_user = "";
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
	_users = new std::vector<User>;
	_banned = new std::vector<User>;
	_users = cpy._users;
	_banned = cpy._banned;
	_topic = cpy._topic;
	_topic_user = cpy._topic_user;
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

std::string				Channel::get_topic_user()
{
	return (_topic_user);
}

void					Channel::set_topic_user(std::string topic_user)
{
	_topic_user = topic_user;
}

std::string				Channel::get_topic_time()
{
	return (_topic_time);
}

void					Channel::set_topic_time(std::string topic_time)
{
	_topic_time = topic_time;
}

User				*Channel::get_operator()
{
	return (_operator);
}

void					Channel::set_operator(User *user)
{
	_operator = user;
}

bool				Channel::banned_user(User *user)
{
	_banned->push_back(*user);
}

bool				Channel::is_banned(User *user)
{
	for (std::vector<User>::iterator ite = _banned->begin(); ite != _banned->end(); ite++)
		if (ite->get_nickname() == user->get_nickname())
			return (true) ;
	return (false);
}