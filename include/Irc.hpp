#ifndef IRC_H
# define IRC_H

# include <sys/socket.h> 
# include <iostream>
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
# include "Command.hpp"
# include "Message.hpp"
# include "User.hpp"

#define ENDLINE "\r\n"

void						dispatch_cmd(std::string buffer, Server *server, User *user);
std::vector<std::string>	split_vector(std::string s, std::string delimiter);
Channel						*find_channel(Server *server, std::string channel_name);
void						add_channel_in_user(Channel *channel, User *user);
bool						check_nickname(std::string nickname, Server *server);
void						print_infos(Server *server);

# define TRUE	1
# define FALSE	0

#endif

