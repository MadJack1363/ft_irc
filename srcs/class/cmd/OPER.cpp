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
	std::string	name;
	std::string	password;

	if (!this->replyPush(user, "OPER " + params))
		return false;
	if (params.empty())
		return this->replyPush(user, "461 OPER :Not enough parameters");
	name = params.substr(0, params.find(' '));
	params.erase(0, name.length()).erase(0, params.find_first_not_of(' '));
	if (params.empty())
		return this->replyPush(user, "461 OPER :Not enough parameters");
	password = params.substr(0, params.find(' '));
	if (this->_config["oper_name"] != name || this->_config["oper_password"] != password)
		return this->replyPush(user, "464 " + user.getNickname() + " :Password incorrect");
	if (user.getModes().find('o') == std::string::npos)
		user.setModes(user.getModes() + 'o');
	return replyPush(user, "221 " + user.getNickname() + " :" + user.getModes())
		&& replyPush(user, "381 " + user.getNickname() + " :You are now an IRC operator.");
}
