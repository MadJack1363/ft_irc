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
	if (this->_channels.count(params) == 0){
		params = params.c_str() + params.find('#') + 1;
		this->_channels.insert(std::make_pair<std::string, Channel>(params, Channel(params)));
	}
	this->_channels[params].addUser(user);

	// Channel &tm = this->_channels[params];

	// Server::logMsg(INTERNAL, "Utilisateur log in " + params);
	// for (std::vector<User *>::const_iterator ite = tm.getUsers().begin(); ite != tm.getUsers().end(); ite++)
	// {
	// 	Server::logMsg(INTERNAL, "\t" + (*ite)->getNickname());
	// }
	return true;
}
