#include "class/Server.hpp"

/**
 * @brief	Send a message either to a channel or to an user.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::PRIVMSG(User &user, std::string &params)//!!!
{
	std::string	target_name = params.substr(0, params.find(' '));
	std::string	msg_send =  params.substr(params.find(' ') + 1, params.length());

	if (*target_name.begin() == '#')
	{
		target_name = target_name.substr(1, target_name.length());
		for (std::map<std::string, Channel>::iterator ite = this->_lookupChannels.begin() ; ite != this->_lookupChannels.end() ; ite++)
		{
			if (ite->second.getName().compare(target_name) == 0){
				for (std::vector<User *>::iterator itv = ite->second.getUsers().begin(); itv != ite->second.getUsers().end(); itv++)
				{
					// Server::replyPush(*(*itv), "PRIVMSG " + msg_send);
					Server::replyPush(*(*itv), "PRIVMSG " + (*itv)->getNickname() + " :"+ user.getNickname() + " " + msg_send);
					Server::replySend(*(*itv));
				}
				return true;
			}
		}
	}
	else
	{
		for (std::list<User>::iterator ite = this->_users.begin() ; ite != this->_users.end() ; ite++)
		{
			if (ite->getNickname().compare(target_name) == 0)
			{
				Server::replyPush(*ite, "PRIVMSG "+ user.getNickname() + " " + msg_send);
				Server::replySend(*ite);
				return true;
			}
		}
	}
	return true;
}
