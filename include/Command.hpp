#ifndef COMMAND_HPP
 # define COMMAND_HPP

 #include "Irc.hpp"

 class Server;

 enum	Command_list
{
	PASS,
	USER,
	NICK,
	CAP,
	PING,
	JOIN,
	PRIVMSG,
	PART,
	TOPIC,
	KICK,
	MODE,
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
