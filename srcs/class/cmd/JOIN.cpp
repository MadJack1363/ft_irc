#include "class/Server.hpp"

/**
 * @brief	Make an user joining a channel.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::JOIN(User &user, std::string &params)
{
	std::vector<std::string>	channel_join;

	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") JOIN " + params);
	params = params.c_str() + params.find('#') + 1;
	while (params.find('#') != std::string::npos)
	{
		channel_join.push_back(params.substr(0, params.find(',')));
		params = params.c_str() + params.find('#') + 1;
	}
	channel_join.push_back(params.substr(0, params.length()));
	for (std::vector<std::string>::iterator ite = channel_join.begin(); ite != channel_join.end(); ite++)
	{
		if (this->_lookupChannels[*ite].getName().compare("Empty") == 0)
			this->_lookupChannels[*ite].setName(*ite);
		this->_lookupChannels[*ite].addUser(user);
	}
	return true;
}
