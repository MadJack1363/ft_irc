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
		return this->replyPush("431 " + user.getNickname() + " :No nickname given");
	if (this->_finder.find(params) != this->_finder.end())
		return this->replyPush("433 " + user.getNickname() + " :Nickname already in use");
	this->_finder.erase(user.getNickname());
	user.setNickname(params);
	this->_finder.insert(std::pair<std::string, User *const>(user.getNickname(), &user));
	return this->replyPush("NICK " + user.getNickname());
}
