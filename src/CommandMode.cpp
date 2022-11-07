#include "../include/Irc.hpp"

void	ExecutionManager::command_mode(std::vector<std::string> line, Client *client)
{
	std::cout << "command Mode" << std::endl;

	Channel				*channel;
	std::string			msg;

	//Affichage
	for (std::vector<std::string>::iterator it = line.begin(); it != line.end(); ++it)
		std::cout << "|" << *it << "|" << std::endl;
	
	std::string		str = (line.begin()+1)->data();
	std::cout << "str = "<< str <<std::endl;
	/* if (parse_channel_name(line)) */
	/* { */
		if ((channel = find_channel(str)) == NULL)
			client->answer = ":server 403 " + str + " No such channel 1" + ENDLINE;
		else
		{
			std::string		mode = "Ooitbmv";
			std::size_t		pos;
			str = (line.begin()+2)->data();
			if ((pos = str.find_first_of(mode, 0)) != std::string::npos)
			{
				std::cout << "pos find = " << pos << " corresponding to = " << str[pos] << std::endl;
			}
			else
			{
				std::cout << "Mode Not found" << std::endl;
			}
		}
	/* } */
	/* else */
		client->answer = ":server 403 " + str + " No such channel 2" + ENDLINE;
}

// void	ExecutionManager::mode_invite(Client *client, Channel *channel, std::vector<std::string> line)
// {
//     if (line.at(2).at(0) == '+')
//         channel->change_invite_only(true);
//     else if (line.at(2).at(0) == '-')
//         channel->change_invite_only(false);
//     else
//         std::cout << "Erreur parsing Mode" <<std::endl;
// }

// void	ExecutionManager::mode_topic(Client *client, Channel *channel, std::vector<std::string> line)
// {
//     if (line.at(2).at(0) == '+')
//         channel->change_mode_topic(true);
//     else if (line.at(2).at(0) == '-')
//         channel->change_mode_topic(false);
//     else
//         std::cout << "Erreur parsing Mode" <<std::endl;
// }

// void	ExecutionManager::mode_moderated(Client *client, Channel *channel, std::vector<std::string> line)
// {
//     if (line.at(2).at(0) == '+')
//         channel->change_moderated(true);
//     else if (line.at(2).at(0) == '-')
//         channel->change_moderated(false);
//     else
//         std::cout << "Erreur parsing Mode" <<std::endl;
// }

// void	ExecutionManager::mode_operator(Client *client, Channel *channel, std::vector<std::string> line)
// {
//     Client  *other_client;

//     if (line.at(2).at(0) == '+')
//     {
//         if ((other_client = find_client(line.at(3).data())) != NULL)
//             channel->add_operator(other_client);
//     }
//     else if (line.at(2).at(0) == '-')
//         channel->change_moderated(false);
//     else
//         std::cout << "Erreur parsing Mode" <<std::endl;
// }

// void	ExecutionManager::mode_voice(Client *client, Channel *channel, std::vector<std::string> line)
// {

// }

// void	ExecutionManager::mode_banned(Client *client, Channel *channel, std::vector<std::string> line)
// {

// }
