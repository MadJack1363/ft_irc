#include "class/Server.hpp"

/**
 * @brief	Set a new nickname for an user.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::NICK(User &user, std::string const &params)
{
	std::string	nickname;

	nickname = params;
	if (nickname.empty())
		return this->replyPush(user, "431 " + user.getNickname() + " :No nickname given");

	if (nickname.find_first_not_of(User::getAvailableNicknameChars()) != std::string::npos ||
		nickname == this->_config["server_name"])
		return this->replyPush(user, "432 " + user.getNickname() + ' ' + nickname + " :Erroneous nickname");

	if (this->_lookupUsers.find(nickname) != this->_lookupUsers.end())
		return this->replyPush(user, "433 " + user.getNickname() + ' ' + nickname + " :Nickname is already in use");

	this->_lookupUsers.erase(user.getNickname());
	user.setNickname(nickname);
	this->_lookupUsers.insert(std::pair<std::string, User *const>(user.getNickname(), &user));
	if (user.getIsRegistered() && !this->replyPush(user, ':' + user.getMask() + " NICK " + params))
		return false;
	user.setMask();
	return true;
}
