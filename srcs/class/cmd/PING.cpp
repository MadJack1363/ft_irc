#include "class/Server.hpp"

/**
 * @brief	Ask the server if the connection is still alive.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::PING(User &user, std::string &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") PING " + params);
	return this->replyPush("PONG " + this->_name);
}
