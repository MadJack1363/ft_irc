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
	std::string				targetName;
	std::string::iterator	it;

	params.erase(0, params.find_first_not_of(' '));
	if (params.empty())
		return this->replyPush("461 MODE :Not enough parameters");
	targetName = params.substr(0, params.find(' '));
	params.erase(0, params.find(' ') + 1).erase(0, params.find_first_not_of(' '));
	if (params.empty())
		return this->replyPush("461 MODE :Not enough parameters");
	if (*targetName.begin() == '#') // channel mode
	{
		
	}
	else // user mode
	{
		if (this->_lookupUsers.find(targetName) == this->_lookupUsers.end())
			return this->replyPush("401 " + targetName + " :No such nick/channel");
		if (targetName != user.getNickname())
			return this->replyPush("502 :Cant change mode for other users");
		for (it = params.begin() ; it != params.end() ; )
		{
			if (*it == '+')
			{
				for (++it ; it != params.end() && *it != ' ' && *it != '-' ; ++it)
				{
					if (user.availableModes().find(*it) == std::string::npos)
						return this->replyPush(std::string("472 ") + *it + " :is unknown mode char to me");
					// if (*it != 'o') // REMIND: uncomment this line when OPER is implemented
						user.addMode(*it);
				}
			}
			else if (*it == '-')
			{
				for (++it ; it != params.end() && *it != ' ' && *it != '+' ; ++it)
				{
					if (user.availableModes().find(*it) == std::string::npos)
						return this->replyPush(std::string("472 ") + *it + " :is unknown mode char to me");
					user.delMode(*it);
				}
			}
			else if (*it == ' ')
				return this->replyPush("501 :Unknown MODE flag");
			else
				++it;
		}
		return this->replyPush("221 " + user.activeModes());
	}
	return true;
}
