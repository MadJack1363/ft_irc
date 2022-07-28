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
	std::string													userList;
	std::string::const_iterator									cit0;
	std::string::const_iterator									cit1;
	std::map<std::string const, User *const>::const_iterator	cit2;
	std::map<std::string const, Channel>::iterator				it;

	for (cit0 = params.begin(), cit1 = params.begin() ; cit1 != params.end() && *cit1 != ' ' && *cit1 != ':' ; ++cit1);
	channelsToJoin = std::string(cit0, cit1);
	if (channelsToJoin.empty())
		return this->replyPush(user, ':' + user.getMask() + " 461 " + user.getNickname() + " JOIN :Not enough parameters");

	if (cit1 != params.end() && *cit1 == ':')
		reason = std::string(cit1 + 1, static_cast<std::string::const_iterator>(params.end()));

	for (cit1 = channelsToJoin.begin() ; cit1 != channelsToJoin.end() ; )
	{
		for (cit0 = cit1 ; cit1 != channelsToJoin.end() && *cit1 != ' ' && *cit1 != ',' ; ++cit1);
		channelName = std::string(cit0, cit1);
		if (*channelName.begin() != '#')
			channelName.insert(channelName.begin(), '#');
		it = this->_lookupChannels.find(channelName);
		if (it == this->_lookupChannels.end())
		{
			this->_lookupChannels.insert(std::pair<std::string const, Channel>(channelName, Channel(channelName)));
			it = this->_lookupChannels.find(channelName);
		}
		if (it->second.find(user.getNickname()) == it->second.end())
		{
			it->second.addUser(user);

			for (cit2 = it->second.begin() ; cit2 != it->second.end() ; ++cit2)
				userList += ' ' + cit2->second->getNickname();

			if (!this->replyPush(user, ':' + user.getMask() + " JOIN " + channelName) ||
				!this->replyPush(user, ':' + user.getMask() + " 332 " + user.getNickname() + ' ' + channelName + " :" + it->second.getTopic()) ||
				!this->replyPush(user, ':' + user.getMask() + " 353 " + user.getNickname() + " = " + channelName + " :" + userList) ||
				!this->replyPush(user, ':' + user.getMask() + " 366 " + user.getNickname() + ' ' + channelName + " :End of /NAMES list"))
				return false;

			for (cit2 = it->second.begin() ; cit2 != it->second.end() ; cit2++)
				if (cit2->second != &user &&
					(!this->replyPush(*cit2->second, ':' + user.getMask() + " JOIN " + channelName) ||
						!this->replySend(*cit2->second)))
					return false;
		}
		if (cit1 != channelsToJoin.end() && *cit1 != ' ')
			++cit1;
	}
	return true;
}
