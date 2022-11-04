#include "../include/Irc.hpp"

Client::Client(void) : wlcm_send(0), _nb_channel(0), _checkPw(0)
{
	answer = "";
	std::cout << "Constructor Client by default called" << std::endl;
	_channels = new std::vector<Channel>;
}

Client::Client(Client const &cpy) : wlcm_send(cpy.wlcm_send), _nb_channel(cpy._nb_channel)
{
	*this = cpy;
	_channels = cpy._channels;
}

Client::~Client(void)
{
	std::cout << "Destructor Client called" << std::endl;
}

Client	&Client::operator=(Client const &src)
{
	this->_name = src._name;
	this->_nickname = src._nickname;
	this->_password = src._password;
	this->answer = src.answer;
	this->wlcm_send = src.wlcm_send;
	this->_nb_channel = src._nb_channel;
	this->_channels = src._channels;
	return (*this);
}

std::string		Client::get_nickname()
{
	return (_nickname);
}

std::string		Client::get_name()
{
	return (_name);
}

std::string		Client::get_password()
{
	return (_password);
}

void			Client::set_checkPw(bool i)
{
	_check_pw = i;
}

void			Client::set_nickname(std::string nickname)
{
	// std::cout << "Set NickName" << std::endl;
	if (_nickname != nickname)
		_nickname = nickname;
}

void			Client::set_name(std::string name)
{
	// std::cout << "Set Name = |" << name << "|" << std::endl;
	if (_name != name)
		_name = name;
}

void			Client::set_password(std::string password)
{
	// std::cout << "Set Password" << std::endl;
	if (_password != password)
		_password = password;
}

int				Client::get_nb_channel()
{
	return (_nb_channel);
}

void			Client::set_nb_channel(int nb_channel)
{
	if (nb_channel != _nb_channel)
		_nb_channel = nb_channel;
}

