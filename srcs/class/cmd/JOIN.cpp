#include "class/Server.hpp"

/**
 * @brief	Make an user joining one or more channel(s).
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::JOIN(User &user, std::string &params)
{
	std::string													channelsToJoin;
	std::string													reason("has joined the channel");
	std::string													channelName;
	std::string::const_iterator									cit0;
	std::string::const_iterator									cit1;
	std::map<std::string const, User *const>::const_iterator	cit2;
	std::map<std::string const, Channel>::iterator				it;

	if (!this->replyPush(user, ':' + user.getMask() + " PART " + params))
		return false;

	for (cit0 = params.begin(), cit1 = params.begin() ; cit1 != params.end() && *cit1 != ' ' && *cit1 != ':' ; ++cit1);
	channelsToJoin = std::string(cit0, cit1);
	if (channelsToJoin.empty())
		return this->replyPush(user, ':' + user.getMask() + " 461 " + user.getNickname() + " PART :Not enough parameters");

	if (cit1 != params.end() && *cit1 == ':')
		reason = std::string(cit1 + 1, static_cast<std::string::const_iterator>(params.end()));

	for (cit1 = channelsToJoin.begin() ; cit1 != channelsToJoin.end() ; )
	{
		for (cit0 = cit1 ; cit1 != channelsToJoin.end() && *cit1 != ' ' && *cit1 != ',' ; ++cit1);
		channelName = std::string(cit0, cit1);
		if (*channelName.begin() == '#')
			channelName.erase(channelName.begin());
		it = this->_lookupChannels.find(channelName);
		if (it == this->_lookupChannels.end())
		{
			// TODO: create the channel
		}
		if (it->second.getLookupUsers().find(user.getNickname()) == it->second.getLookupUsers().end())
		{
			if (!this->replyPush(user, ':' + user.getMask() + " JOIN " + channelName) ||
				!this->replyPush(user, ':' + user.getMask() + " 332 " + user.getNickname() + ' ' + channelName + " :" + it->second.getTopic()))
				return false;
			for (cit2 = it->second.getLookupUsers().begin() ; cit2 != it->second.getLookupUsers().end() ; ++cit2)
			{
				if (!this->replyPush(*cit2->second, ':' + user.getMask() + " PART " + channelName + " :" + reason) ||
					!this->replySend(*cit2->second))
					return false;
			}
			it->second.delUser(user.getNickname());
			if (it->second.getLookupUsers().empty())
				this->_lookupChannels.erase(it);
		}
		if (cit1 != channelsToJoin.end() && *cit1 != ' ')
			++cit1;
	}
	return true;
}
