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
	std::string	channelName;
	std::string	userToKick;
	std::string	reason;

	if (!this->replyPush(user, "KICK " + params))
		return false;
	if (params.empty())
		return this->replyPush(user, "461 KICK :Not enough parameters");
	channelName = params.substr(0, params.find(' ')).erase(0, 1);
	params.erase(0, params.find(' ') + 1).erase(0, params.find_first_not_of(' '));
	if (params.empty())
		return this->replyPush(user, "461 KICK :Not enough parameters");
	userToKick = params.substr(0, params.find(' '));
	params.erase(0, params.find(':') + 1);
	reason = params;

	if(this->_lookupChannels.find(channelName) == this->_lookupChannels.end())
		return this->replyPush(user, "403 KICK " + channelName + " :No such channel");

	// TODO message from user to userToKick
	// TODO kick userToKick from the channel
	return true;
}
