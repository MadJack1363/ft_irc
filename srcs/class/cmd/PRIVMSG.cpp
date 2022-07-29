#include "class/Server.hpp"

/**
 * @brief	Send a message either to a channel or to an user.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::PRIVMSG(User &user, std::string const &params)
{
	std::string													targets;
	std::string													text;
	std::string													targetName;
	std::string::const_iterator									cit0;
	std::string::const_iterator									cit1;
	std::map<std::string const, Channel>::const_iterator		cit2;
	std::map<std::string const, User *const>::const_iterator	cit3;

	for (cit0 = params.begin(), cit1 = params.begin() ; cit1 != params.end() && *cit1 != ' ' && *cit1 != ':' ; ++cit1);
	targets = std::string(cit0, cit1);
	if (targets.empty())
		return this->replyPush(user, "411 " + user.getNickname() + " :No recipent given PRIVMSG");

	for ( ; cit1 != params.end() && *cit1 != ':' ; ++cit1);
	if (cit1 == params.end())
		return this->replyPush(user, "412 " + user.getNickname() + " :No text to send");
	text = std::string(cit1 + 1, static_cast<std::string::const_iterator>(params.end()));

	for (cit1 = targets.begin() ; cit1 != targets.end() ; ++cit1)
	{
		for (cit0 = cit1 ; cit1 != targets.end() && *cit1 != ',' ; ++cit1);
		targetName = std::string(cit0, cit1);

		if (*targetName.begin() == '#') // message to channel
		{
			cit2 = this->_lookupChannels.find(targetName);
			if (cit2 == this->_lookupChannels.end())
			{
				if (!this->replyPush(user, "401 " + user.getNickname() + ' ' + targetName + " :No such nick/channel"))
					return false;
			}
			else
			{
				for (cit3 = cit2->second.begin() ; cit3 != cit2->second.end() ; cit3++)
					if (cit3->second != &user &&
						(!this->replyPush(*cit3->second, ':' + user.getMask() + " PRIVMSG " + targetName + " :" + text) ||
							!this->replySend(*cit3->second)))
					return false;
			}
		}
		else // message to user
		{
			cit3 = this->_lookupUsers.find(targetName);
			if (cit3 == this->_lookupUsers.end() || cit3->second->getModes().find('i') != std::string::npos)
			{
				if (!this->replyPush(user, "401 " + user.getNickname() + ' ' + targetName + " :No such nick/channel"))
					return false;
			}
			else if (cit3->second->getModes().find('a') != std::string::npos)
			{
				if (!this->replyPush(user, "301 " + user.getNickname() + ' ' + targetName + " :" + cit3->second->getAwayMsg()))
					return false;
			}
			else
			{
				if (!this->replyPush(*cit3->second, ':' + user.getMask() + " PRIVMSG " + targetName + " :" + text) ||
					!this->replySend(*cit3->second))
					return false;
			}
		}
		if (cit1 == targets.end())
			break ;
	}
	return true;
}
