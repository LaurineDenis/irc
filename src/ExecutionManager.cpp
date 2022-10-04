#include "../include/Irc.hpp"
#include <cstring>
#include <sys/poll.h>
#include <sys/socket.h>
#include <type_traits>

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
	this->_users->push_back(*user);
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
	this->newUser(newSocket);
	this->_clientSd.at(0).revents = 0;
	this->_clientSd.at(this->_clientSd.size() - 1).revents = POLLIN;
}

void		ExecutionManager::deleteUser(int i)
{
	/* supprimer user des channels */
	this->_users->erase(this->_users->cbegin() + i);
	/* close connection? */
}

void		ExecutionManager::dispatchCmd(User *user, std::string buffer)
{
	std::vector<std::string>	out;
	std::string					cmd_name[10] = {"NICK", "USER", "CAP", "JOIN", "PRIVMSG", "PART", "TOPIC", "KICK", "PING", "QUIT"};
	int							i;
	// ExecutionManager			manager;

	out = split_vector(buffer, " \r\n");
	/* print_infos(server); */
	std::cout << "User cmd : " << user->get_nickname() << std::endl;
	std::cout << "Commande Split :" << std::endl;
	for (std::vector<std::string>::iterator it = out.begin(); it != out.end(); ++it)
		std::cout << "|" << *it << "|" << std::endl;
	for (i = 0; i < 10; i++)
	{
		if (cmd_name[i] == out[0])
			break ;
	}
	switch (i)
	{
		case CAP :
			std::cout << "Cap switch" << std::endl;
			command_cap(out);
			break;
		case PING :
			std::cout << "Ping switch" << std::endl;
			command_ping(out);
			break;
		case NICK :
			std::cout << "Nick switch" << std::endl;
			command_nick(out, user);
			break;
		case USER :
			std::cout << "User switch" << std::endl;
			command_user(out, user);
			break;
		case JOIN :
			std::cout << "Join switch" << std::endl;
			command_join(out, user, buffer);
			break;
		case PRIVMSG :
			std::cout << "Privmsg switch" << std::endl;
			command_privmsg(out, user);
			break;
		case PART :
			std::cout << "Part switch" << std::endl;
			command_part(out, user);
			break;
		case TOPIC :
			std::cout << "Topic switch" << std::endl;
			command_topic(out, user);
			break;
		case KICK :
			std::cout << "Kick switch" << std::endl;
			command_kick(out, user);
			break;
		case QUIT :
			std::cout << "Quit switch" << std::endl;
			command_quit(out, user);
			break;
		default :
			std::cout << "Unknow command" << std::endl;
	}
}

void		ExecutionManager::sendRpl()
{
	std::string		answer;

	for (int i = 1; i < this->_clientSd.size(); i++)
	{
		if (this->_users->at(i - 1).answer.length())
		{
			answer = this->_users->at(i - 1).answer + ENDLINE;
			std::cout <<  "Reply sent: " << answer.c_str() << std::endl;
			send(this->_clientSd.at(i).fd, answer.c_str(), answer.length(), 0);
			this->_users->at(i - 1).answer = "";
		}
	}
}

std::string		ExecutionManager::recvCmd(int i)
{
	std::string		cmd;
	ssize_t			ret = 1;
	char			buffer[4096];

	while (cmd.find(ENDLINE, 0) == std::string::npos && ret > 0)
	{
		ret += recv(this->_clientSd.at(i).fd, buffer, sizeof(buffer), 0);
		buffer[ret] = 0;
		cmd = buffer;
	}
	this->_clientSd.at(i).revents = 0;
	return (cmd);
}

void		ExecutionManager::IO_Operation()
{
	ssize_t			ret;
	std::string		cmd;

	for (int i = 1; i < this->_clientSd.size(); i++)
	{
		cmd = "";
		cmd = recvCmd(i);
		if (!cmd.size() && this->_clientSd.at(i).events & POLLHUP)
		{
			std::cout << "User " << this->_clientSd.at(i).fd << " disconnected!" << std::endl;
			this->deleteUser(i);
		}
		else
		{
			if (cmd.length())
				this->dispatchCmd(&this->_users->at(i - 1), cmd);
			this->sendRpl();
		}
	}
}
