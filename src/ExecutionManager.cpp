#include "../include/Irc.hpp"
#include <cstdlib>
#include <cstring>
#include <sys/poll.h>
#include <sys/socket.h>
#include <type_traits>

void	ExecutionManager::addSd(int socket, short events)
{
	struct pollfd	add = {socket, events, 0};

	this->_clientSd.push_back(add);
}

void	ExecutionManager::newClient(int newSocket)
{
	Client	*client = new Client();

	this->_clientAmount++;
	fcntl(newSocket, F_SETFL, O_NONBLOCK);
	this->_clients->push_back(*client);
	this->addSd(newSocket, POLLIN);
}

void		ExecutionManager::init_cmd()
{
	this->_cmd_name = new std::vector<std::string>;
	_cmd_name->push_back("PASS");
	_cmd_name->push_back("CAP");
	_cmd_name->push_back("USER");
	_cmd_name->push_back("NICK");
	_cmd_name->push_back("PING");
	_cmd_name->push_back("JOIN");
	_cmd_name->push_back("PRIVMSG");
	_cmd_name->push_back("PART");
	_cmd_name->push_back("TOPIC");
	_cmd_name->push_back("KICK");
	_cmd_name->push_back("MODE");
	_cmd_name->push_back("INVITE");
	_cmd_name->push_back("QUIT");
}

void		ExecutionManager::init(Server *server)
{
	this->_clients = new std::vector<Client>;
	this->_channels = new std::vector<Channel>;
	this->_password = server->getPw();
	this->addSd(server->getSocket(), POLLIN);
	this->_address = "127.0.0.1";
	init_cmd();
}

int			ExecutionManager::checkPoll()
{
	int ret;

	ret = poll(this->_clientSd.data(), this->_clientSd.size(), -1);
	if (ret > 0)
	{
		if (this->_clientSd.at(0).revents & POLLIN)
			return 1;
		else
			return 2;
	}
	return -1;
}

void		ExecutionManager::newConnection()
{
	int					newSocket;
	struct	sockaddr_in	newClient;

	newSocket = accept(this->_clientSd.at(0).fd, (struct sockaddr *)&newClient, &this->_addrlen);
	std::cout << "New Connection: " << newSocket << std::endl;
	this->newClient(newSocket);
	this->_clientSd.at(0).revents = 0;
	this->_clientSd.at(this->_clientSd.size() - 1).revents = POLLIN;
}

void		ExecutionManager::deleteClient(int i)
{
	Client	*client = &this->_clients->at(i);
	Channel	*channel;
	while (this->_clients->at(i)._channels->size())
	{
		channel = &this->_channels->at(0);
		send_msg_to_channel_clients(":" + client->get_nickname() + "!" + client->get_nickname() + "@server PART " + channel->get_name() + ENDLINE, client, channel);
		remove_client_of_channel(channel, client);
	}
	this->_clients->erase(this->_clients->cbegin() + i);
	close(this->_clientSd.at(i + 1).fd);
	this->_clientSd.erase(this->_clientSd.cbegin() + i + 1);
}

int			ExecutionManager::is_command(std::vector<std::string> line)
{
	int		i = 0;
	for (std::vector<std::string>::iterator it = _cmd_name->begin(); it != _cmd_name->end(); ++it, i++)
	{
		if (it->data() == line[0])
			return (i);
	}
	return (-1);
}

void		ExecutionManager::parseCmd(Client *client, std::string buffer, int index)
{
	std::vector<std::string>	line = split(buffer, " \r\n");
	int							cmd;

	if ((cmd = is_command(line)) >= 0)
	{
		if (!client->is_register(cmd))
		{
			std::cerr << "registration error mamene" << std::endl;
			return;
		}
		dispatchCmd(client, line, index, cmd);
	}
	else
	{
		std::cout << "Command not found cmd = " << cmd << std::endl;
	}
}

void		ExecutionManager::dispatchCmd(Client *client, std::vector<std::string> line, int index, int cmd)
{
	std::cout << cmd << std::endl;
	switch (cmd)
	{
		case PASS :
			command_pass(line, client);
			break;
		case CAP :
			command_cap(line);
			break;
		case USER :
			command_client(line, client);
			break;
		case NICK :
			command_nick(line, client);
			break;
		case PING :
			command_ping(line, client);
			break;
		case JOIN :
			command_join(line, client);
			break;
		case PRIVMSG :
			command_privmsg(line, client);
			break;
		case PART :
			command_part(line, client);
			break;
		case TOPIC :
			command_topic(line, client);
			break;
		case KICK :
			command_kick(line, client);
			break;
		case MODE :
			command_mode(line, client);
			break;
		case INVITE :
			command_invite(line, client);
			break;
		case QUIT :
			command_quit(client, index);
			break;
		default :
			std::cout << "Unknow command" << std::endl;
	}
}

void		ExecutionManager::shutdown()
{
	std::cout << "Shutting down server ..." << std::endl;
	_clients->clear();
	_channels->clear();
	close(_clientSd.at(0).fd);
	_clientSd.clear();
}

void		ExecutionManager::sendRpl()
{
	std::string		answer;

	for (int i = 1; i < this->_clientSd.size(); i++)
	{
		if (this->_clients->at(i - 1).answer.length())
		{
			answer = this->_clients->at(i - 1).answer + ENDLINE;
			std::cout <<  "Reply sent: " << answer.c_str() << std::endl;
			send(this->_clientSd.at(i).fd, answer.c_str(), answer.length(), 0);
			this->_clients->at(i - 1).answer = "";
		}
	}
}

ssize_t		ExecutionManager::recvCmd(int i)
{
	std::string		cmd;
	ssize_t			ret = 0;
	char			buffer[512] = {0};

	cmd = this->_clients->at(i - 1).get_cmd();
	ret = recv(this->_clientSd.at(i).fd, buffer, sizeof(buffer), 0);
	buffer[ret] = 0;
	cmd += buffer;
	this->_clientSd.at(i).revents = 0;
	this->_clients->at(i - 1).set_cmd(cmd);
	if (cmd.find(ENDLINE) != std::string::npos)
		return 1;
	else if (ret == 0 && !cmd.size())
		return -1;
	return 0;
}

void		ExecutionManager::IO_Operation()
{
	ssize_t						to_process;
	ssize_t						ret;
	std::vector<std::string>	split_cmd;

	for (int i = 1; i < this->_clientSd.size(); i++)
	{
		to_process = this->recvCmd(i);
		if (to_process == -1)
		{
			std::cout << "Client " << this->_clientSd.at(i).fd << " disconnected!" << std::endl;
			command_quit(&this->_clients->at(i - 1), i - 1);
		}
		else if (to_process)
		{
			split_cmd = split(_clients->at(i - 1).get_cmd(), "\r\n");
			this->_clients->at(i - 1).set_cmd("");
			for (std::vector<std::string>::iterator it = split_cmd.begin(); it != split_cmd.end(); ++it)
			{
				std::cout << "CMD = |" << it->data() << "|" << std::endl;
				this->parseCmd(&this->_clients->at(i - 1), it->data(), i - 1);
				this->sendRpl();
				if (_clients->at(i - 1)._del == 1)
					deleteClient(i - 1);
			}
		}
		else 
			continue;
	}
}
