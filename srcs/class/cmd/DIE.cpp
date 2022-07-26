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
	// if (!this->replyPush(user, "DIE " + params))
	// 	return false;
	if (user.getModes().find('o') == std::string::npos)
		return this->replyPush(user, "481 " + user.getNickname() + " :Permission Denied - You're not an IRC operator");
	this->_state = STOPPED;
	return true;
}
