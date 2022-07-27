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
	std::string					nickname;
	std::string					reason;
	std::string					subParams;
	std::string::const_iterator	cit0;
	std::string::const_iterator	cit1;

	for (cit0 = params.begin(), cit1 = params.begin() ; cit1 != params.end() && *cit1 != ' ' ; ++cit1);
	nickname = std::string(cit0, cit1);
	if (nickname.empty())
		return this->replyPush(user, "461 " + user.getNickname() + " KILL :Not enough parameters");

	for ( ; cit1 != params.end() && *cit1 == ' ' ; ++cit1);
	if (cit1 == params.end() || *cit1 != ':')
		return this->replyPush(user, "461 " + user.getNickname() + " KILL :Not enough parameters");
	reason = std::string(cit1 + 1, static_cast<std::string::const_iterator>(params.end()));

	if (user.getModes().find('o') == std::string::npos)
		return this->replyPush(user, "481 " + user.getNickname() + " :Permission Denied - You're not an IRC operator");
	if (this->_lookupUsers.find(nickname) == this->_lookupUsers.end())
	{
		if (nickname == this->_config["server_name"])
			return this->replyPush(user, "483 " + user.getNickname() + " :You can't kill a server!");
		return this->replyPush(user, "401 " + user.getNickname() + ' ' + nickname + " :No such nick/channel");
	}

	User	&userToKill = *this->_lookupUsers.find(nickname)->second;
	Server::addToBanList(userToKill);

	this->replyPush(userToKill, ":" + user.getMask() + " KILL " + userToKill.getNickname() + " " + reason);

	subParams = ":Killed (" + user.getNickname() + " (" + reason + "))";
	return this->QUIT(userToKill, subParams);
}
