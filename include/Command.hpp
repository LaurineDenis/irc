#ifndef COMMAND_HPP
 # define COMMAND_HPP

 #include "Irc.hpp"

 class Server;

 enum	Command_lst
{
	NICK,
	USER,
	CAP,
	JOIN,
	PRIVMSG,
	PART,
	TOPIC,
	KICK,
	PING,
	QUIT
};

 class Command
{
	public:

			Command();
			~Command();
    
    private:
        void    execute();

};

#endif
