#include "class/Server.hpp"

/**
 * @brief	Disconnect an user from the server.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::QUIT(User &user, std::string &params)
{
	std::string	reason;

	if (!this->replyPush(user, "Error :Connection terminated by dwarf") ||
		!this->replySend(user))
		return false;

	if (!user.getLookupChannels().empty())
	{
		reason = params;
		if (*reason.begin() == ':')
			reason.erase(reason.begin());
		reason = "Quit: " + reason;

		std::list<User *>	usersToNotice;

		for (std::map<std::string const, Channel *const>::const_iterator citChan = user.getLookupChannels().begin() ; citChan != user.getLookupChannels().end() ; citChan++)
		{
			for (std::map<std::string const, User *const>::const_iterator citUser = citChan->second->begin(); citUser != citChan->second->end(); citUser++)
			{
				if (citUser->second != &user && std::find(usersToNotice.begin(), usersToNotice.end(), citUser->second) == usersToNotice.end())
					usersToNotice.push_back(citUser->second);
			}
			citChan->second->delUser(user.getNickname());
		}
		
		for (std::list<User *>::const_iterator cit = usersToNotice.begin(); cit != usersToNotice.end(); cit++)
		{
			if (!this->replyPush(**cit, ":" + user.getMask() + " QUIT :" + reason) ||
				!this->replySend(**cit))
				return false;
		}
	}

	close(user.getSocket());
	user.setSocket(-1);
	return true;
}
