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
	std::string					targetName;
	std::string::const_iterator	cit;
	std::string::size_type		pos;

	if (!this->replyPush(user, "MODE " + params))
		return false;
	if (params.empty())
		return this->replyPush(user, "461 MODE :Not enough parameters");
	targetName = params.substr(0, params.find(' '));
	params.erase(0, targetName.length()).erase(0, params.find_first_not_of(' '));
	if (params.empty())
		return this->replyPush(user, "461 MODE :Not enough parameters");
	if (*targetName.begin() == '#') // channel mode
	{
		return true;
	}
	else // user mode
	{
		if (this->_lookupUsers.find(targetName) == this->_lookupUsers.end())
			return this->replyPush(user, "401 " + targetName + " :No such nick/channel");
		if (targetName != user.getNickname())
			return this->replyPush(user, "502 :Cant change mode for other users");
		for (cit = params.begin() ; cit != params.end() ; )
		{
			if (*cit == '+')
			{
				for (++cit ; cit != params.end() && *cit != ' ' && *cit != '-' ; ++cit)
				{
					if (User::getAvailableModes().find(*cit) == std::string::npos)
						return this->replyPush(user, "472 " + user.getNickname() + ' ' + *cit + " :is unknown mode char to me");
					if (*cit != 'o' && user.getModes().find(*cit) == std::string::npos)
						user.setModes(user.getModes() + *cit);
				}
			}
			else if (*cit == '-')
			{
				for (++cit ; cit != params.end() && *cit != ' ' && *cit != '+' ; ++cit)
				{
					if (User::getAvailableModes().find(*cit) == std::string::npos)
						return this->replyPush(user, "472 " + user.getNickname() + ' ' + *cit + " :is unknown mode char to me");
					pos = user.getModes().find(*cit);
					if (pos != std::string::npos)
						user.setModes(std::string(user.getModes()).erase(pos, 1));
				}
			}
			else if (*cit != ' ')
				return this->replyPush(user, "501 " + user.getNickname() + " :Unknown MODE flag");
			else
				++cit;
		}
		return this->replyPush(user, "221 " + user.getNickname() + " :" + user.getModes());
	}
}
