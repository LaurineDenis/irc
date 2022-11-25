#ifndef IRC_H
# define IRC_H

# include <sys/socket.h> 
# include <iostream>
# include <signal.h>
# include <sys/types.h>
# include <sys/socket.h> 
# include <poll.h> 
# include <netinet/in.h>
# include <fcntl.h>
# include <vector>
# include <string>
# include <unistd.h>
# include <time.h>
# include "Server.hpp"
# include "Channel.hpp"
# include "Client.hpp"
# include "ExecutionManager.hpp"
# include "NumericReply.hpp"
// # include "Command/Cap.hpp"
// # include "Command/Join.hpp"
// # include "Command/Kick.hpp"
// # include "Command/Nick.hpp"
// # include "Command/Part.hpp"
// # include "Command/Pass.hpp"
// # include "Command/Ping.hpp"
// # include "Command/Privmsg.hpp"
// # include "Command/Quit.hpp"
// # include "Command/Topic.hpp"
// # include "Command/User.hpp"

#define ENDLINE "\r\n"

std::vector<std::string>	split(std::string s, std::string delimiter);
bool						add_channel_in_client(Channel *channel, Client *client);
bool						is_in_channel(Channel *channel, Client *client);

# define TRUE	1
# define FALSE	0

#endif

