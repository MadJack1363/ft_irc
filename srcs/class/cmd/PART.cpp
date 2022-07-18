#include "class/Server.hpp"

/**
 * @brief	Make an user leaving a channel.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::PART(User &user, std::string &params)
{
	std::vector<std::string>	channel_left;
	Channel						tmp;

	params = params.c_str() + params.find(':') + 1;
	while (params.find(',') != std::string::npos)
	{
		channel_left.push_back(params.substr(0, params.find(',')));
		params = params.c_str() + params.find(',') + 1;
	}
	channel_left.push_back(params.substr(0, params.find(',')));
	for(std::vector<std::string>::iterator ite = channel_left.begin();ite != channel_left.end();ite++)
	{
		// config for send a custom message or no of all user of any channel
		tmp = this->_lookupChannels[*ite];
		// if (tmp.getUsers().size() == 1)
		// 	// need to delete the channel
		// else{
		// 	// send value to all User inside the channel
		// }
	}
	return true;
}
