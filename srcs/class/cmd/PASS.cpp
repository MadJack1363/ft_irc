#include "class/Server.hpp"

/**
 * @brief	Check if a provided password is correct to connect to the server.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::PASS(User &user, std::string const &params)
{
	if (user.getIsRegistered())
		return this->replyPush(user, "462 " + user.getNickname() + " :You may not reregister");
	if (params.empty())
		return this->replyPush(user, "461 PASS :not enough parameters");
	user.setPassword(params);
	return true;
}
