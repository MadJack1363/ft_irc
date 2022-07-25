#include "class/Server.hpp"

/**
 * @brief	Set a new username, hostname and realname for an user.
 * 			Set the user mask accordingly.
 * 			If everything is correct, the command set the user as registered.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::USER(User &user, std::string &params)
{
	if (user.getIsRegistered())
		return this->replyPush(user, "462 :You may not reregister");

	if (params.empty())
		return this->replyPush(user, "461 " + user.getNickname() + " USER :Not enough parameters");
	user.setUsername(params.substr(0, params.find(' ')));

	params.erase(0, user.getUsername().length()).erase(0, params.find_first_not_of(' '));
	if (params.empty())
		return this->replyPush(user, "461 " + user.getNickname() + " USER :Not enough parameters");
	user.setHostname(params.substr(0, params.find(' ')));

	params.erase(0, user.getHostname().length()).erase(0, params.find_first_not_of(' '));
	if (params.empty())
		return this->replyPush(user, "461 " + user.getNickname() + " USER :Not enough parameters");
	user.setServname(params.substr(0, params.find(' ')));

	params.erase(0, user.getServname().length()).erase(0, params.find_first_not_of(' '));
	if (params.empty() || params.erase(params.begin()) == params.end())
		return this->replyPush(user, "461 " + user.getNickname() + " USER :Not enough parameters");
	user.setRealname(params);

	if (!this->_config["server_password"].empty() && this->_config["server_password"] != user.getPassword())
		return this->replyPush(user, "464 " + user.getNickname() + " :Password incorrect");
	user.setIsRegistered(true);
	user.setMask();

	return this->replyPush(user, "001 " + user.getNickname() + " :Welcome to the Mine, " + user.getMask() + '.')
		&& this->replyPush(user, "002 " + user.getNickname() + " :Your host is " + this->_config["server_name"] + ", running version " + this->_config["server_version"] + '.')
		&& this->replyPush(user, "003 " + user.getNickname() + " :This server was created " + this->_creationTime + '.')
		&& this->replyPush(user, "004 " + user.getNickname() + " :" + this->_config["server_name"] + " " + this->_config["server_version"] + ' ' + User::getAvailableModes() + ' ' + Channel::availableModes() + '.');
}
