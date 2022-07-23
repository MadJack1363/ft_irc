#include <class/Server.hpp>

/**
 * @brief	Query for informations about a specific user.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::WHOIS(User &user, std::string &params)
{
	std::map<std::string, User *const>::const_iterator	cit;
	std::string											nickname;

	if (!this->replyPush(user, "WHOIS " + params))
		return false;
	if (params.empty())
		return this->replyPush(user, "431 " + user.getNickname() + " :Not enough parameters");
	nickname = params.substr(0, params.find(' '));
	cit = this->_lookupUsers.find(nickname);
	if (cit == this->_lookupUsers.end())
		return this->replyPush(user, "401 " + user.getNickname() + ' ' + nickname + " :No such nick/channel");
	return this->replyPush(user, "307 " + user.getNickname() + ' ' + cit->second->getNickname() + " :has identified for this nick")
		&& this->replyPush(user, "311 " + user.getNickname() + ' ' + cit->second->getNickname() + ' ' + cit->second->getUsername() + ' ' + cit->second->getServname() + " * :" + cit->second->getRealname())
		&& (cit->second->getModes().find('o') == std::string::npos
			|| this->replyPush(user, "313 " + user.getNickname() + ' ' + cit->second->getNickname() + " :is an IRC operator"))
		&& this->replyPush(user, "379 " + user. getNickname() + ' ' + cit->second->getNickname() + " :is using modes " + cit->second->getModes())
		&& this->replyPush(user, "318 " + user. getNickname() + ' ' + cit->second->getNickname() + " :End of /WHOIS list.");
}
