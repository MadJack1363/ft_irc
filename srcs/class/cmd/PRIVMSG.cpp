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
	// FIXME Have to recode this function
	// FIXME CHECK THE VALUE THE SERVER SEND BACK

	// TODO Check the replies 
	// ERR_NOSUCHNICK (401)
	// ERR_NOSUCHSERVER (402)
	// ERR_CANNOTSENDTOCHAN (404)
	// ERR_TOOMANYTARGETS (407)
	// ERR_NORECIPIENT (411)
	// ERR_NOTEXTTOSEND (412)
	// ERR_NOTOPLEVEL (413)
	// ERR_WILDTOPLEVEL (414)
	// RPL_AWAY (301)
	std::string tmp;
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
					if (user.getNickname() != (*itv)->getNickname())
					{
						// From Documentation
						//   :dan!~h@localhost PRIVMSG #coolpeople :Hi everyone!
						// ; Message from dan to the channel #coolpeople
						// :flo!florian@AB969147.54975EF1.B6CE2A61.IP PRIVMSG #TesT :Salut
						tmp = ":" + user.getNickname() + "!" + user.getUsername() + "@" + this->_config["host"] + " PRIVMSG #" + target_name + " " + msg_send;
						(*itv)->setMsg(tmp);
						Server::replySend(*(*itv));
					}
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
				// From Documentation
				// :Angel PRIVMSG Wiz :Hello are you receiving this message ?
				// ; Message from Angel to Wiz.

				tmp = ":" + user.getNickname() + " PRIVMSG " + target_name + " " + msg_send;
				ite->setMsg(tmp);
				Server::replySend(*ite);

				return true;
			}
		}
	}
	return true;
}
