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
	// return this->replyPush(user, ':' + /* user.getMask() */ this->_config["host"] + " PING " + params)
	// 	&& this->replyPush(user, ':' + /* user.getMask() */ this->_config["host"] + " PONG " + params);
	return this->replyPush(user, ':' + /* user.getMask() */ this->_config["host"] + " PONG " + params);
}
