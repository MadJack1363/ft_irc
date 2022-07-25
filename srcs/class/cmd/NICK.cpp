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
	if (params.empty())
		return this->replyPush(user, "431 " + user.getNickname() + " :No nickname given");
	if (this->_lookupUsers.find(params) != this->_lookupUsers.end())
		return this->replyPush(user, "433 " + user.getNickname() + " " + params + " :Nickname is already in use");
	this->_lookupUsers.erase(user.getNickname());
	user.setNickname(params);
	this->_lookupUsers.insert(std::pair<std::string, User *const>(user.getNickname(), &user));
	return this->replyPush(user, "NICK " + params);
}
