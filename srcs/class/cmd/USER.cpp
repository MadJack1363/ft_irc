#include "class/Server.hpp"

/**
 * @brief	Set a new username, hostname and realname for an user.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::USER(User &user, std::string &params)
{
	if (user.getIsRegistered())
		return this->replyPush("462 :You may not reregister");
	if (params.empty())
		return this->replyPush("461 USER :Not enough parameters");
	user.setUsername(params.substr(0, params.find(' ')));
	params.erase(0, params.find(' ') + 1).erase(0, params.find_first_not_of(' '));
	if (params.empty())
		return this->replyPush("461 USER :Not enough parameters");
	user.setHostname(params.substr(0, params.find(' ')));
	params.erase(0, params.find(' ') + 1).erase(0, params.find_first_not_of(' '));
	if (params.empty())
		return this->replyPush("461 USER :Not enough parameters");
	params.erase(0, params.find(' ') + 1).erase(0, params.find_first_not_of(' '));
	if (params.empty())
		return this->replyPush("461 USER :Not enough parameters");
	params.erase(params.begin());
	user.setRealname(params);
	if (!this->_password.empty() && this->_password != user.getPassword())
		return this->replyPush("464 " + user.getNickname() + " :Password incorrect");
	user.setIsRegistered(true);
	return this->replyPush("001 " + user.getNickname() + " :Welcome to the Mine.")
		&& this->replyPush("002 " + user.getNickname() + " :Your host is " + this->_name + ", running version " + this->_version + '.')
		&& this->replyPush("003 " + user.getNickname() + " :This server was created " + this->_creationTime + '.')
		&& this->replyPush("004 " + user.getNickname() + " :" + this->_name + " " + this->_version + ' ' + User::availableModes() + ' ' + Channel::availableModes() + '.');
}
