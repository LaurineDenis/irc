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
# include "Server.hpp"
# include "Channel.hpp"
# include "Message.hpp"
# include "User.hpp"
# include "ExecutionManager.hpp"

#define ENDLINE "\r\n"

std::vector<std::string>	split_vector(std::string s, std::string delimiter);
void						add_channel_in_user(Channel *channel, User *user);
bool						is_in_channel(Channel *channel, User *user);

# define TRUE	1
# define FALSE	0

#endif

