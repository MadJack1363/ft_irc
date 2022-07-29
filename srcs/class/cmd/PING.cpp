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
	if (params.empty())
		return this->replyPush(user, "461 " + user.getNickname() + " PING :Not enough parameters");
	return this->replyPush(user, "PONG " + params);
}

// REMIND Modif the emplacement of the function
/**
 * @brief	Send a ping to User
 * 
 * @param user the user to check is alive
 * 
 * @return true is send work, false otherwise.
 */
bool Server::checkStillAlive(User &user)
{
	if (!this->replyPush(user, "PING " + user.getNickname()) ||
		!this->replySend(user))
		return false;
	user.setWaitingForPong(TIMEOUT);
	return true;
}
