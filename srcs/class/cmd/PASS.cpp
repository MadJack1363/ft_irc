#include "class/Server.hpp"

/**
 * @brief	Check if a provided password is correct to connect to the server.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::PASS(User &user, std::string &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") PASS " + params);
	if (user.getIsRegistered())
		return this->replyPush("462 :You may not reregister");
	if (params.empty())
		return this->replyPush("461 PASS :not enough parameters");
	user.setPassword(params);
	return true;
}
