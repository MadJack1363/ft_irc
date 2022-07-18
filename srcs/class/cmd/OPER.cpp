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

	if (!this->replyPush("OPER " + params))
		return false;
	if (params.empty())
		return this->replyPush("461 OPER :Not enough parameters");
	name = params.substr(0, params.find(' '));
	params.erase(0, params.find(' ') + 1).erase(0, params.find_first_not_of(' '));
	if (params.empty())
		return this->replyPush("461 OPER :Not enough parameters");
	password = params.substr(0, params.find(' '));
	if (this->_config["oper_name"] != name || this->_config["oper_password"] != password)
		return this->replyPush("464 " + user.getNickname() + " :Password incorrect");
	user.setModes(user.getModes() | (1 << User::OPERATOR));
	return replyPush("221 " + user.getNickname() + " :" + user.activeModes())
		&& replyPush("381 " + user.getNickname() + " :You are now an IRC operator.");
}
