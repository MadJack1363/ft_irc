#include "class/Server.hpp"

/**
 * @brief	Set a new nickname for an user.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::NICK(User &user, std::string &params)
{
	std::map<int, User>::iterator	it;

	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") NICK " + params);
	if (params.empty())
		return this->reply(user, "431 " + user.getNickname() + " :No nickname given");
	for (it = this->_users.begin() ;
		it != this->_users.end() && it->second.getNickname() != params ;
		++it);
	if (it != this->_users.end())
		return this->reply(user, "433 " + user.getNickname() + " :Nickname already in use");
	user.setNickname(params);
	if (!user.getIsRegistered())
		return true;
	return this->reply(user, "NICK " + user.getNickname());
}
