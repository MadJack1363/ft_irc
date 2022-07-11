#include "class/Server.hpp"

/**
 * @brief	Send a message either to a channel or to an user.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::PRIVMSG(User &user, std::string &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") PRIVMSG " + params);

	std::string	recipient = params.substr(0, params.find(':') - 1);
	std::string	msg_send =  params.substr(params.find(':') + 1, params.length());

	Server::replyPush(msg_send);
	for (std::map<int, User>::iterator ite = this->_users.begin() ; ite != this->_users.end() ; ite++)
	{
		if (ite->second.getNickname().compare(recipient) == 0){
			Server::replySend(ite->second);
			return true;
		}
	}
	for (std::map<std::string, Channel>::iterator ite = this->_channels.begin() ; ite != this->_channels.end() ; ite++)
	{
		if (ite->second.getName().compare(recipient)){
			for (std::vector<User *>::iterator itv = ite->second.getUsers().begin(); itv != ite->second.getUsers().end(); itv++)
				Server::replySend(*(*itv));
			return true;
		}
	}
	return true;
}
