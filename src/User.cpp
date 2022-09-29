#include "../include/Irc.hpp"

User::User(void) : wlcm_send(0), _nb_channel(0)
{
	answer = "";
	std::cout << "Constructor User by default called" << std::endl;
	_channels = new std::vector<Channel>;
}

User::User(User const &cpy) : wlcm_send(0), _nb_channel(0)
{
	*this = cpy;
	_channels = new std::vector<Channel>;
}

User::~User(void)
{
	std::cout << "Destructor User called" << std::endl;
}

User	&User::operator=(User const &src)
{
	this->_name = src._name;
	this->_nickname = src._nickname;
	this->_password = src._password;
	this->answer = src.answer;
	this->wlcm_send = src.wlcm_send;
	this->_nb_channel = src._nb_channel;
	return (*this);
}

std::string		User::get_nickname()
{
	return (_nickname);
}

std::string		User::get_name()
{
	return (_name);
}

std::string		User::get_password()
{
	return (_password);
}

void			User::set_nickname(std::string nickname)
{
	// std::cout << "Set NickName" << std::endl;
	if (_nickname != nickname)
		_nickname = nickname;
}

void			User::set_name(std::string name)
{
	// std::cout << "Set Name = |" << name << "|" << std::endl;
	if (_name != name)
		_name = name;
}

void			User::set_password(std::string password)
{
	// std::cout << "Set Password" << std::endl;
	if (_password != password)
		_password = password;
}

int				User::get_nb_channel()
{
	return (_nb_channel);
}

void			User::set_nb_channel(int nb_channel)
{
	if (nb_channel != _nb_channel)
		_nb_channel = nb_channel;
}
