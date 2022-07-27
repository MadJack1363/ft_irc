#include "class/Server.hpp"

/**
 * @brief	Kick an user from a channel.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::KICK(User &user, std::string &params)
{
	std::string					channelName;
	std::string					usernameToKick;
	std::string					reason("Speaking elvish language.");
	std::string::const_iterator	cit0;
	std::string::const_iterator	cit1;

	if (!this->replyPush(user, "KICK " + params))
		return false;

	for (cit0 = params.begin(), cit1 = params.begin() ; cit1 != params.end() && *cit1 != ' ' ; ++cit1);
	channelName = std::string(cit0, cit1);
	if (channelName.empty())
		return this->replyPush(user, ':' + user.getMask() + " 461 " + user.getNickname() + " KICK :Not enough parameters");

	for ( ; cit1 != params.end() && *cit1 == ' ' ; ++cit1);
	for (cit0 = cit1 ; cit1 != params.end() && *cit1 != ' ' ; ++cit1);
	usernameToKick = std::string(cit0, cit1);
	if (usernameToKick.empty())
		return this->replyPush(user, ':' + user.getMask() + " 461 " + user.getNickname() + " KICK :Not enough parameters");

	for ( ; cit1 != params.end() && *cit1 == ' ' ; ++cit1);
	if (cit1 != params.end() && *cit1 == ':')
		reason = std::string(cit1 + 1, static_cast<std::string::const_iterator>(params.end()));

	if(this->_lookupChannels.find(channelName) == this->_lookupChannels.end())
		return this->replyPush(user, ':' + user.getMask() + " 403 " + user.getNickname() + ' ' + channelName + " :No such channel");

	Channel	&chan = this->_lookupChannels.find(channelName)->second;

	if (this->_lookupUsers.find(usernameToKick) == this->_lookupUsers.end())
		return this->replyPush(user, ':' + user.getMask() + " 441 " + user.getNickname() + ' ' + usernameToKick + ' ' + channelName + " :They aren't on that channel");

	// TODO message from user to userToKick

	chan.delUser(usernameToKick);
	return true;
}
