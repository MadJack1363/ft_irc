#include <class/Server.hpp>

/**
 * @brief	Query for informations about a specific user.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::WHOIS(User &user, std::string const &params)
{
	std::string											nickname;
	std::string::const_iterator							cit0;
	std::string::const_iterator							cit1;
	std::map<std::string, User *const>::const_iterator	cit2;

	for (cit0 = params.begin(), cit1 = params.begin() ; cit1 != params.end() && *cit1 != ' ' ; ++cit1);
	nickname = std::string(cit0, cit1);
	if (nickname.empty())
		return this->replyPush(user, "431 " + user.getNickname() + " :No nickname given");

	cit2 = this->_lookupUsers.find(nickname);
	if (cit2 == this->_lookupUsers.end())
		return this->replyPush(user, "401 " + user.getNickname() + ' ' + nickname + " :No such nick/channel");
	return this->replyPush(user, "307 " + user.getNickname() + ' ' + cit2->second->getNickname() + " :has identified for this nick")
		&& this->replyPush(user, "311 " + user.getNickname() + ' ' + cit2->second->getNickname() + ' ' + cit2->second->getUsername() + ' ' + cit2->second->getServname() + " * :" + cit2->second->getRealname())
		&& (cit2->second->getModes().find('o') == std::string::npos
			|| this->replyPush(user, "313 " + user.getNickname() + ' ' + cit2->second->getNickname() + " :is an IRC operator"))
		&& this->replyPush(user, "379 " + user. getNickname() + ' ' + cit2->second->getNickname() + " :is using modes " + cit2->second->getModes())
		&& this->replyPush(user, "318 " + user. getNickname() + ' ' + cit2->second->getNickname() + " :End of /WHOIS list.");
}
