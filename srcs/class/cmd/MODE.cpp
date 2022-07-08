#include "class/Server.hpp"

inline static bool	__channelMode(User &user, std::string &params)
{
	return true;
}

inline static bool	__userMode(User &user, std::string &params)
{
	return true;
}

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
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") MODE " + params);

	params.erase(0, params.find(' ') + 1);
	if (params.empty())
		return this->reply(user, "461 MODE :Not enough parameters");
	if (params[0] == '#')
		return __channelMode(user, params);
	return __userMode(user, params);
}
