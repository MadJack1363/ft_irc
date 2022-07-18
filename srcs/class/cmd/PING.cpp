#include "class/Server.hpp"

/**
 * @brief	Ask the server if the connection is still alive.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::PING(User &user, std::string &params)// FIXME Have to fix CMD PING
{
	(void)params;
	return this->replyPush(user, "PONG :" + this->_config["server_name"]);
}
