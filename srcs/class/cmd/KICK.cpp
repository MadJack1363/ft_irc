#include "class/Server.hpp"

/**
 * @brief	Kick an user from a channel.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::KICK(User &user, std::string const &params)
{
	std::string					channelName;
	std::string					usernameToKick;
	std::string					reason("Speaking elvish language.");
	std::string::const_iterator	cit0;
	std::string::const_iterator	cit1;

	for (cit0 = params.begin(), cit1 = params.begin() ; cit1 != params.end() && *cit1 != ' ' ; ++cit1);
	channelName = std::string(cit0, cit1);
	if (channelName.empty())
		return this->replyPush(user, "461 " + user.getNickname() + " KICK :Not enough parameters");

	for ( ; cit1 != params.end() && *cit1 == ' ' ; ++cit1);
	for (cit0 = cit1 ; cit1 != params.end() && *cit1 != ' ' ; ++cit1);
	usernameToKick = std::string(cit0, cit1);
	if (usernameToKick.empty())
		return this->replyPush(user, "461 " + user.getNickname() + " KICK :Not enough parameters");

	for ( ; cit1 != params.end() && *cit1 != ':' ; ++cit1);
	if (cit1 + 1 != params.end())
		reason = std::string(cit1 + 1, static_cast<std::string::const_iterator>(params.end()));

	if(this->_lookupChannels.find(channelName) == this->_lookupChannels.end())
		return this->replyPush(user, "403 " + user.getNickname() + ' ' + channelName + " :No such channel");

	Channel	&chan = this->_lookupChannels.find(channelName)->second;

	if (this->_lookupUsers.find(usernameToKick) == this->_lookupUsers.end())
		return this->replyPush(user, "441 " + user.getNickname() + ' ' + usernameToKick + ' ' + channelName + " :They aren't on that channel");

	if (user.getModes().find('o') == std::string::npos)
		return this->replyPush(user, "481 " + user.getNickname() + " :Permission Denied - You're not an IRC operator");

	User	&userToKick = *this->_lookupUsers.find(usernameToKick)->second;
	if (!this->replyPush(userToKick, ":" + userToKick.getMask() + " PART " + channelName))
		return false;
	for (std::map<std::string const, User *const>::const_iterator cit = chan.begin(); cit != chan.end(); cit++)
	{
		this->replyPush(*cit->second, ":" + user.getMask() + " KICK " + channelName + " " + usernameToKick + " :" + reason);
		this->replySend(*cit->second);
	}
	
	chan.delUser(usernameToKick);
	return true;
}
