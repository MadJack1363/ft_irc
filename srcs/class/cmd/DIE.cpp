#include "class/Server.hpp"

/**
 * @brief	Terminate the IRC server.
 * 			This command is reserved for IRC operators.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::DIE(User &user, std::string &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") DIE " + params);
	return true;
}
