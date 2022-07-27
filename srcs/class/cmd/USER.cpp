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
	std::string					username;
	std::string					hostname;
	std::string					servname;
	std::string					realname;
	std::string::const_iterator	cit0;
	std::string::const_iterator	cit1;

	if (user.getIsRegistered())
		return this->replyPush(user, ':' + user.getMask() + " 462 :You may not reregister");

	for (cit0 = params.begin(), cit1 = params.begin() ; cit0 != params.end() && *cit1 != ' ' ; ++cit1);
	username = std::string(cit0, cit1);

	if (username.empty())
		return this->replyPush(user, ':' + user.getMask() + " 461 " + user.getNickname() + " USER :Not enough parameters");
	user.setUsername(username);

	for ( ; cit1 != params.end() && *cit1 == ' ' ; ++cit1);
	for (cit0 = cit1 ; cit1 != params.end() && *cit1 != ' ' ; ++cit1);
	hostname = std::string(cit0, cit1);
	if (hostname.empty())
		return this->replyPush(user, ':' + user.getMask() + " 461 " + user.getNickname() + " USER :Not enough parameters");
	user.setHostname(hostname);

	for ( ; cit1 != params.end() && *cit1 == ' ' ; ++cit1);
	for (cit0 = cit1 ; cit1 != params.end() && *cit1 != ' ' ; ++cit1);
	servname = std::string(cit0, cit1);
	if (servname.empty())
		return this->replyPush(user, ':' + user.getMask() + " 461 " + user.getNickname() + " USER :Not enough parameters");
	user.setServname(servname);

	for ( ; cit1 != params.end() && *cit1 == ' ' ; ++cit1);
	realname = std::string(cit0, static_cast<std::string::const_iterator>(params.end()));
	if (realname.empty())
		return this->replyPush(user, ':' + user.getMask() + " 461 " + user.getNickname() + " USER :Not enough parameters");
	if (*realname.begin() == ':')
	{
		realname.erase(realname.begin());
		if (realname.empty())
			return this->replyPush(user, ':' + user.getMask() + " 461 " + user.getNickname() + " USER :Not enough parameters");
	}
	else if (realname.find(' ') != std::string::npos)
		realname.erase(realname.find(' '));
	user.setRealname(realname);

	if (!this->_config["server_password"].empty() && this->_config["server_password"] != user.getPassword())
		return this->replyPush(user, ':' + user.getMask() + " 464 " + user.getNickname() + " :Password incorrect");
	user.setIsRegistered(true);
	user.setMask();

	std::string	motdParams("");

	return this->replyPush(user, "001 " + user.getNickname() + " :Welcome to the Mine, " + user.getMask() + '.')
		&& this->replyPush(user, "002 " + user.getNickname() + " :Your host is " + this->_config["server_name"] + ", running version " + this->_config["server_version"] + '.')
		&& this->replyPush(user, "003 " + user.getNickname() + " :This server was created " + this->_creationTime + '.')
		&& this->replyPush(user, "004 " + user.getNickname() + " :" + this->_config["server_name"] + " " + this->_config["server_version"] + ' ' + User::getAvailableModes() + ' ' + Channel::getAvailableModes() + '.')
		&& MOTD(user, motdParams);
}
