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
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") USER " + params);
	if (user.getIsRegistered())
		return this->reply(user, "462 :You may not register");
	if (params.empty())
		return this->reply(user, "461 USER :Not enough parameters");
	user.setUsername(params.substr(0, params.find(' ')));
	params.erase(0, params.find(' ') + 1).erase(0, params.find_first_not_of(' '));
	if (params.empty())
		return this->reply(user, "461 USER :Not enough parameters");
	user.setHostname(params.substr(0, params.find(' ')));
	params.erase(0, params.find(' ') + 1).erase(0, params.find_first_not_of(' '));
	if (params.empty())
		return this->reply(user, "461 USER :Not enough parameters");
	params.erase(0, params.find(' ') + 1).erase(0, params.find_first_not_of(' '));
	if (params.empty())
		return this->reply(user, "461 USER :Not enough parameters");
	params.erase(params.begin());
	user.setRealname(params);
	if (!this->_password.empty() && this->_password != user.getPassword())
		return this->reply(user, "464 :Password incorrect");
	user.setIsRegistered(true);
	return this->reply(user, "001 " + user.getNickname() + " :Welcome to the Mine " + user.getNickname() + '!' + user.getUsername() + '@' + this->_name + '.')
		&& this->reply(user, "002 " + user.getNickname() + " :Your host is " + this->_name + ", running version " + this->_version + '.')
		&& this->reply(user, "003 " + user.getNickname() + " :This server was created " + this->_creationTime + '.')
		&& this->reply(user, "004 " + user.getNickname() + " :" + this->_name + ' ' + this->_version + ' ' + this->_availableUserModes + ' ' + this->_availableChannelModes + '.');
}
