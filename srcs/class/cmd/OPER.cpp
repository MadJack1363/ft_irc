#include "class/Server.hpp"

/**
 * @brief	Make an user being promoted to operator status.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::OPER(User &user, std::string &params)
{
	std::string					name;
	std::string					password;
	std::string::const_iterator	cit0;
	std::string::const_iterator	cit1;

	for (cit0 = params.begin(), cit1 = params.begin() ; cit1 != params.end() && *cit1 != ' ' ; ++cit1);
	name = std::string(cit0, cit1);
	if (name.empty())
		return this->replyPush(user, "461 " + user.getNickname() + " OPER :Not enough parameters");

	for ( ; cit1 != params.end() && *cit1 == ' ' ; ++cit1);
	password = std::string(cit1, static_cast<std::string::const_iterator>(params.end()));
	if (password.empty())
		return this->replyPush(user, "461 " + user.getNickname() + " OPER :Not enough parameters");

	if (this->_config["oper_name"] != name || this->_config["oper_password"] != password)
		return this->replyPush(user, "464 " + user.getNickname() + " :Password incorrect");
	if (user.getModes().find('o') == std::string::npos)
		user.setModes(user.getModes() + 'o');
	return replyPush(user, "221 " + user.getNickname() + " :" + user.getModes())
		&& replyPush(user, "381 " + user.getNickname() + " :You are now an IRC operator.");
}
