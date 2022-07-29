#include "class/Server.hpp"
#include "ft.hpp"

/**
 * @brief	Change the modes of either an user or a channel.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::MODE(User &user, std::string const &params)
{
	static std::string const								delimiter("+- ");
	std::string												targetName;
	std::string												modeString;
	std::string												modeArgs;
	std::string::size_type									pos;
	std::string::const_iterator								cit0;
	std::string::const_iterator								cit1;
	std::map<std::string const, Channel>::iterator			it;

	for (cit0 = params.begin(), cit1 = params.begin() ; cit1 != params.end() && *cit1 != ' ' ; ++cit1);
	targetName = std::string(cit0, cit1);
	if (targetName.empty())
		return this->replyPush(user, "461 " + user.getNickname() + " MODE :Not enough parameters");

	for ( ; cit1 != params.end() && *cit1 == ' ' ; ++cit1);
	for (cit0 = cit1 ; cit1 != params.end() ; ++cit1)
	{
		if (*cit1 != '+' && *cit1 != '-')
			break ;
		for ( ; cit1 != params.end() && *cit1 != ' ' ; ++cit1);
		if (cit1 == params.end())
			break ;
	}
	modeString = std::string(cit0, cit1);

	for ( ; cit1 != params.end() && *cit1 == ' ' ; ++cit1);
	modeArgs = std::string(cit1, static_cast<std::string::const_iterator>(params.end()));

	if (*targetName.begin() == '#') // channel mode
	{
		it = this->_lookupChannels.find(targetName);
		if (it == this->_lookupChannels.end())
			return this->replyPush(user, "403 " + user.getNickname() + ' ' + targetName + " :No such channel");
		if (!modeString.empty())
		{
			// REMIND: This operator check would be changed into a chanops check, if we decide to implement channel modes someday.
			if (user.getModes().find('o') == std::string::npos)
				return this->replyPush(user, "482 " + user.getNickname() + ' ' + targetName + " :You're not channel operator");
			for (cit0 = modeString.begin() ; cit0 != modeString.end() ; )
			{
				if (*cit0 == '+')
				{
					for (++cit0 ; cit0 != modeString.end() && delimiter.find(*cit0) == std::string::npos ; ++cit0)
					{
						if (Channel::getAvailableModes().find(*cit0) == std::string::npos)
						{
							if (!this->replyPush(user, "472 " + user.getNickname() + ' ' + *cit0 + " :is unknown mode char to me"))
								return false;
						}
						else if (it->second.getModes().find(*cit0) == std::string::npos)
							it->second.setModes(it->second.getModes() + *cit0);
					}
				}
				else if (*cit0 == '-')
				{
					for (++cit0 ; cit0 != modeString.end() && delimiter.find(*cit0) == std::string::npos ; ++cit0)
					{
						if (Channel::getAvailableModes().find(*cit0) == std::string::npos)
						{
							if (!this->replyPush(user, "472 " + user.getNickname() + ' ' + *cit0 + " :is unknown mode char to me"))
								return false;
						}
						pos = it->second.getModes().find(*cit0);
						if (pos != std::string::npos)
							it->second.setModes(std::string(user.getModes()).erase(pos, 1));
					}
				}
				else if (*cit0 != ' ')
					return this->replyPush(user, "501 " + user.getNickname() + " :Unknown MODE flag");
				else
					++cit0;
			}
			
		}
		return this->replyPush(user, "324 " + user.getNickname() + ' ' + targetName + ' ' + modeString);
	}
	else // user mode
	{
		if (!modeString.empty())
		{
			if (this->_lookupUsers.find(targetName) == this->_lookupUsers.end())
				return this->replyPush(user, "401 " + user.getNickname() + ' ' + targetName + " :No such nick/channel");
			if (targetName != user.getNickname())
				return this->replyPush(user, "502 " + user.getNickname() + " :Cant change mode for other users");
			for (cit0 = modeString.begin() ; cit0 != modeString.end() ; )
			{
				if (*cit0 == '+')
				{
					for (++cit0 ; cit0 != modeString.end() && delimiter.find(*cit0) == std::string::npos ; ++cit0)
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
					for (++cit0 ; cit0 != modeString.end() && delimiter.find(*cit0) == std::string::npos ; ++cit0)
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
