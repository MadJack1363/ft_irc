#include "class/Server.hpp"

/**
 * @brief	Change the modes of either an user or a channel.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::MODE(User &user, std::string &params)
{
	static std::string const	delimiter("+- ");
	std::string					targetName;
	std::string					modeChanges;
	std::string::size_type		pos;
	std::string::const_iterator	cit0;
	std::string::const_iterator	cit1;

	for (cit0 = params.begin(), cit1 = params.begin() ; cit1 != params.end() && *cit1 != ' ' ; ++cit1);
	targetName = std::string(cit0, cit1);
	if (targetName.empty())
		return this->replyPush(user, "461 " + user.getNickname() + " MODE :Not enough parameters");

	for ( ; cit1 != params.end() && *cit1 == ' ' ; ++cit1);
	modeChanges = std::string(cit1, static_cast<std::string::const_iterator>(params.end()));

	if (*targetName.begin() == '#') // channel mode
	{
		return true;
	}
	else // user mode
	{
		if (!modeChanges.empty())
		{
			if (this->_lookupUsers.find(targetName) == this->_lookupUsers.end())
				return this->replyPush(user, "401 " + user.getNickname() + ' ' + targetName + " :No such nick/channel");
			if (targetName != user.getNickname())
				return this->replyPush(user, "502 " + user.getNickname() + " :Cant change mode for other users");
			for (cit0 = modeChanges.begin() ; cit0 != modeChanges.end() ; )
			{
				if (*cit0 == '+')
				{
					for (++cit0 ; cit0 != modeChanges.end() && delimiter.find(*cit0) == std::string::npos ; ++cit0)
					{
						if (User::getAvailableModes().find(*cit0) == std::string::npos)
						{
							if (!this->replyPush(user, "472 " + user.getNickname() + ' ' + *cit0 + " :is unknown mode char to me"))
								return false;
						}
						else if (*cit0 != 'o' && user.getModes().find(*cit0) == std::string::npos)
							user.setModes(user.getModes() + *cit0);
					}
				}
				else if (*cit0 == '-')
				{
					for (++cit0 ; cit0 != modeChanges.end() && delimiter.find(*cit0) == std::string::npos ; ++cit0)
					{
						if (User::getAvailableModes().find(*cit0) == std::string::npos)
						{
							if (!this->replyPush(user, "472 " + user.getNickname() + ' ' + *cit0 + " :is unknown mode char to me"))
								return false;
						}
						pos = user.getModes().find(*cit0);
						if (pos != std::string::npos)
							user.setModes(std::string(user.getModes()).erase(pos, 1));
					}
				}
				else if (*cit0 != ' ')
					return this->replyPush(user, "501 " + user.getNickname() + " :Unknown MODE flag");
				else
					++cit0;
			}
		}
		return this->replyPush(user, "221 " + user.getNickname() + " :" + user.getModes());
	}
}
