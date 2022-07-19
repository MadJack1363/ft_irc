#include "class/Server.hpp"

/**
 * @brief	Update data of an user.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::SET(User &user, std::string &params)
{
	// REMIND: Temporary to silent unused parameter warning
	(void)user;
	(void)params;
	return true;
}
