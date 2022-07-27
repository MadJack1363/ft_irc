#include "class/Server.hpp"

// /**
//  * @brief Config the message to send form CMD Privmsg
//  * 
//  * @param user the people to send the reply
//  * @param channel_name the name of the channel he join
//  * @return void
//  */
// void	Server::privmsgSend(User &user, std::string &target)
// {

// 	return true;
// }

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
	// DO Check the replies
	// FIX ERR_NOSUCHNICK (401)
	// FIX ERR_CANNOTSENDTOCHAN (404)
	// DO ERR_TOOMANYTARGETS (407) | Check but no max persmissions
	std::string tmp;
	std::string	target_name = params.substr(0, params.find(' '));
	std::string	msg_send =  params.substr(params.find(':') + 1, params.length());

	if (*target_name.begin() == '#')
	{
		target_name = target_name.substr(1, target_name.length());
		std::map<std::string, Channel>::iterator ite;
		for (ite = this->_lookupChannels.begin() ; ite != this->_lookupChannels.end() ; ite++)
		{
			if (ite->second.getName().compare(target_name) == 0){
				for (std::map<std::string const, User *const>::const_iterator itv = ite->second.begin(); itv != ite->second.end(); itv++)
				{
					if (user.getNickname() != itv->second->getNickname())
					{
						tmp = ":" + user.getNickname() + "!" + user.getUsername() + "@" + this->_config["host"] + " PRIVMSG #" + target_name + " :" + msg_send;
						itv->second->setMsg(tmp);
						Server::replySend(*itv->second);
					}
				}
				return true;
			}
		}
		if (ite == this->_lookupChannels.end())
			return this->replyPush(user, "401 " + user.getNickname() + " :No such nick/channel");
	}
	else
	{
		std::list<User>::iterator	ite;
		for (ite = this->_users.begin() ; ite != this->_users.end() ; ite++)
		{
			if (ite->getNickname().compare(target_name) == 0)
			{
				tmp = ":" + user.getNickname() + " PRIVMSG " + target_name + " :" + msg_send;
				ite->setMsg(tmp);
				Server::replySend(*ite);
				return true;
			}
		}
		if (ite == this->_users.end())
			return this->replyPush(user, "401 " + user.getNickname() + " :No such nick/channel");
	}
	return true;
}
