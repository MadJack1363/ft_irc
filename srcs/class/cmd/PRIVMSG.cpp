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
	std::string	recipient = params.substr(0, params.find(':') - 1);
	std::string	msg_send =  params.substr(params.find(':') + 1, params.length());

	Server::replyPush(user, msg_send);
	for (std::list<User>::iterator ite = this->_users.begin() ; ite != this->_users.end() ; ite++)
	{
		if (ite->getNickname().compare(recipient) == 0)
		{
			Server::replySend(*ite);
			return true;
		}
	}
	for (std::map<std::string, Channel>::iterator ite = this->_lookupChannels.begin() ; ite != this->_lookupChannels.end() ; ite++)
	{
		if (ite->second.getName().compare(recipient))
		{
			for (std::map<std::string const, User *const>::const_iterator cit = ite->second.getLookupUsers().begin(); cit != ite->second.getLookupUsers().end(); cit++)
				Server::replySend(*cit->second);
			return true;
		}
	}
	return true;
}
