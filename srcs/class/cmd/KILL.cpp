#include "class/Server.hpp"

/**
 * @brief	Remove an user from the network.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::KILL(User &user, std::string &params)
{
	std::string	nickname;
	std::string	comment;

	if (!this->replyPush(user, "KILL " + params))
		return false;
	if (params.empty())
		return this->replyPush(user, "461 KILL :Not enough parameters");
	nickname = params.substr(0, params.find(' '));
	params.erase(0, params.find(':') + 1);
	if (params.empty())
		return this->replyPush(user, "461 KILL :Not enough parameters");
	comment = params;
	if (user.getModes().find('o') == std::string::npos)
		return this->replyPush(user, "481 " + user.getNickname() + " :Permission Denied - You're not an IRC operator");
	if (this->_lookupUsers.find(nickname) == this->_lookupUsers.end())
	{
		if (nickname == this->_config["server_name"])
			return this->replyPush(user, "483 " + user.getNickname() + " :You can't kill a server!");
		return this->replyPush(user, "401 KILL " + nickname + " :No such nick/channel");
	}

	User	*userToKill = this->_lookupUsers.find(nickname)->second;
	Server::addToBanList(*userToKill);

	// TODO: message to the user

	if (userToKill == &user)
		user.setMsg("");
	close(userToKill->getSocket());
	userToKill->setSocket(-1);
	this->_lookupUsers.erase(userToKill->getNickname());
	return true;
}
