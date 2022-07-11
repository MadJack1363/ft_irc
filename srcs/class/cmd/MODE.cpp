#include "class/Server.hpp"

/**
 * @brief	Change the privileges of either an user or a channel.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::MODE(User &user, std::string &params)
{
	std::string	targetName;

	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") MODE " + params);
	params.erase(0, params.find(' ') + 1);
	if (params.empty())
		return this->replyPush("461 MODE :Not enough parameters");
	targetName = params.substr(0, params.find(' '));
	if (*targetName.begin() == '#') // channel mode
	{

	}
	else // user mode
	{

	}
	return true;
}
