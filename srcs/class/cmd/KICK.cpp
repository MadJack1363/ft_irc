#include "class/Server.hpp"

/**
 * @brief	Kick an user from a channel.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::KICK(User &user, std::string &params)
{
	// Temporary to silent unused parameter warning
	(void)user;
	(void)params;
	return true;
}
