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
		// FIXME Send a message of all user in the channel with PRIVMSG
		// FIX probably need to modif my join
		if (this->_lookupChannels[*ite].getName().compare("Empty") == 0){
			this->_lookupChannels[*ite].setName(*ite);
		}
		// MEMO Probably need to add the name of the user inside the message
		// std::string	tmp = "#" + *ite + " " + user.getNickname() + " :has joined #" + *ite;
		std::string	tmp = "#" + *ite + " :has joined #" + *ite;
		this->PRIVMSG(user, tmp);
		this->_lookupChannels[*ite].addUser(user);
	}
	return true;
}
