#include "class/Server.hpp"

/**
 * @brief	Remove an user from the network.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::KILL(User &user, std::string const &params)
{
	std::string					nickname;
	std::string					reason("Being an elf");
	std::string					subParams;
	std::string::const_iterator	cit0;
	std::string::const_iterator	cit1;

	for (cit0 = params.begin(), cit1 = params.begin() ; cit1 != params.end() && *cit1 != ' ' ; ++cit1);
	nickname = std::string(cit0, cit1);
	if (nickname.empty())
		return this->replyPush(user, "461 " + user.getNickname() + " KILL :Not enough parameters");

	for ( ; cit1 != params.end() && *cit1 != ':' ; ++cit1);
	if (cit1 == params.end())
		return this->replyPush(user, "461 " + user.getNickname() + " KILL :Not enough parameters");
	if (cit1 + 1 != params.end())
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

	if (!this->replyPush(userToKill, ":" + user.getMask() + " KILL " + userToKill.getNickname() + " :" + reason))
		return false;
	if (!userToKill.getLookupChannels().empty())
	{
		std::list<User *>	usersToNotice;

		reason = "Killed (" + user.getNickname() + " (" + reason + "))";

		for (std::map<std::string const, Channel *const>::const_iterator itChan = userToKill.getLookupChannels().begin(); itChan != userToKill.getLookupChannels().end(); itChan++)
		{
			for (std::map<std::string const, User *const>::const_iterator itUser = itChan->second->begin(); itUser != itChan->second->end(); itUser++)
			{
				if (std::find(usersToNotice.begin(), usersToNotice.end(), itUser->second) == usersToNotice.end())
					usersToNotice.push_back(itUser->second);
			}
			itChan->second->delUser(userToKill.getNickname());
		}

		for (std::list<User *>::const_iterator cit = usersToNotice.begin(); cit != usersToNotice.end(); cit++)
		{
			if (!this->replyPush(**cit, ":" + userToKill.getMask() + " QUIT :" + reason) ||
				!this->replySend(**cit))
				return false;
		}
	}
	if (!this->replyPush(userToKill, "Error :Closing Link: " + this->_config["server_name"] + " (" + reason + ")") ||
		!this->replySend(userToKill))
		return false;

	close(userToKill.getSocket());
	userToKill.setSocket(-1);
	return true;
}
