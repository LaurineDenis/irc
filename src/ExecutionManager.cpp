#include "../include/Irc.hpp"
#include <sys/poll.h>
#include <sys/socket.h>

void	ExecutionManager::addSd(int socket, short events)
{
	struct pollfd	add = {socket, events, 0};

	this->_clientSd.push_back(add);
}

void	ExecutionManager::newUser(int newSocket)
{
	User	*user = new User();

	this->_clientAmount++;
	fcntl(newSocket, F_SETFL, O_NONBLOCK);
	this->_users.push_back(*user);
	this->addSd(newSocket, POLLIN);
}

void		ExecutionManager::init(Server *server)
{
	this->_users = new std::vector<User>;
	this->_channels = new std::vector<Channel>;
	this->addSd(server->getSocket(), POLLIN);
	this->_address = "127.0.0.1";
}

int			ExecutionManager::checkPoll()
{
	int ret;

	ret = poll(this->_clientSd.data(), this->_clientSd.size(), -1);
	if (ret > 0)
	{
		if (this->_clientSd.at(0).events)
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
	this->newUser(newSocket);
	this->_clientSd.at(0).revents = 0;
}

void		ExecutionManager::deleteUser(int i)
{
	/* supprimer user des channels */
	this->_users.erase(this->_users.cbegin() + i);
	/* close connection? */
}

void		ExecutionManager::dispatchCmd()
{
	/* dispatch command from command.cpp with some small changes */
}

void		ExecutionManager::sendRpl()
{
	std::string		answer;

	for (int i = 1; i < this->_clientSd.size(); i++)
	{
		if (this->_users.at(i - 1)._answer.length())
		{
			answer = this->_users.at(i - 1)._answer + ENDLINE;
			std::cout << answer.c_str() << std::endl;
			send(this->_clientFd.at(i), answer.c_str(), answer.length(), 0);
		}
	}
}

std::string		ExecutionManager::recvCmd(int i)
{
	std::string		cmd;
	ssize_t			ret;

	while (cmd.find("\n\r", 0) == std::string::npos)
		recv(this->_clientSd.at(i).fd, cmd.c_str() + cmd.length(), cmd.length(), 0);
	return cmd;
}

void		ExecutionManager::IO_Operation()
{
	ssize_t			ret;
	std::string		cmd;

	for (int i = 1; i < this->_clientSd.size(); i++)
	{
		cmd = recvCmd(i);
		if (!cmd.size() && this->_clientSd.at(i).events & POLLHUP)
		{
			std::cout << "User " << this->_clientSd.at(i).fd << " disconnected!" << std::endl;
			this->deleteUser(i);
		}
		else
		{
			this->dispatchCmd();
			this->sendRpl();
		}
	}
}
