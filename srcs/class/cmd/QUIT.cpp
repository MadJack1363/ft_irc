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

		for (std::map<std::string const, Channel *const>::const_iterator itChan = user.getLookupChannels().begin(); itChan != user.getLookupChannels().end(); itChan++)
		{
			for (std::map<std::string const, User *const>::const_iterator itUser = itChan->second->begin(); itUser != itChan->second->end(); itUser++)
			{
				if (itUser->second != &user && (!this->replyPush(*itUser->second, ":" + user.getMask() + " QUIT :" + reason) ||
					!this->replySend(*itUser->second)))
					return false;
			}
		}
	}

	close(user.getSocket());
	user.setSocket(-1);
	return true;
}
