#include "class/Server.hpp"

/**
 * @brief	Remove an user from the network.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::KILL(User &user, std::string &params)
{
	// REMIND: Temporary to silent unused parameter warning
	(void)user;
	(void)params;
	return true;
}
