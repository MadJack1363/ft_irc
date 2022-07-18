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
bool	Server::DIE(User &user, std::string &params __attribute__((unused)))
{
	if (!(user.getModes() & (1 << User::OPERATOR)))
		return this->replyPush(user, "481 :Permission Denied - You're not an IRC operator");
	this->_state = STOPPED;
	return true;
}
