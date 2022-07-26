#include "class/Server.hpp"

/**
 * @brief Config the message to send form CMD part
 * 
 * @param user the people to send the reply
 * @param channel_name the name of the channel he part
 * @return void
 */
void	Server::partSend(User &user, std::string &channel_name, std::string &message_left)
{
	std::string tmp = ":" + user.getNickname() + "!" + user.getUsername() + "@" + this->_config["host"] + " PART #" + channel_name + " :" + message_left;
	user.setMsg(tmp);
	return ;
}

/**
 * @brief	Make an user leaving a channel.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::PART(User &user, std::string &params)
{
	std::vector<std::string>	channel_left;
	std::string					left_message = " has left the channel";

	// TODO Check if with params const check if compile
	if(params.empty())
		return this->replyPush(user, ':' + user.getMask() + " 461 PART :Not enough parameters");
	params = params.c_str() + params.find(':') + 1;
	if (params.compare("0") == 0)
	{
		for (std::map<std::string, Channel>::iterator itm = this->_lookupChannels.begin(); itm != this->_lookupChannels.end(); itm++)
		{
			if (std::find(itm->second.getUsers().begin(), itm->second.getUsers().end(), &user) != itm->second.getUsers().end())
				channel_left.push_back(itm->second.getName());
		}
	}
	else
	{
		while (params.find(',') != std::string::npos)
		{
			channel_left.push_back(params.substr(0, params.find(',')));
			params = params.c_str() + params.find(',') + 1;
		}
		channel_left.push_back(params.substr(0, params.find(' ')));
		params = params.c_str() + params.find(' ');
		if (params.length() > 1)
			left_message = params;
	}
	try {
		for(std::vector<std::string>::iterator ite = channel_left.begin();ite != channel_left.end();ite++)
		{
			if (this->_lookupChannels.find(*ite) == this->_lookupChannels.end())
				return this->replyPush(user, ':' + user.getMask() + " 403 " + *ite + " :No such channel");
			else
			{
				Channel	&myChan = this->_lookupChannels[*ite];
				if (std::find(myChan.getUsers().begin(), myChan.getUsers().end(), &user) == myChan.getUsers().end())
				{
					this->replyPush(user, ':' + user.getMask() + " 442 " + *ite + " :You're not on that channel");
				}
				else
				{
					if (myChan.getUsers().size() == 1)
					{
						this->_lookupChannels.erase(*ite);
					}
					else
					{
						myChan.delUser(user);
						std::string	cpy = *ite + left_message;
						// FIXME PUTAIN DE PRIVMSG QUI MARCHE PAS
						PRIVMSG(user, cpy);
					}
					partSend(user, *ite, left_message);
					if (ite != channel_left.end())
						this->replySend(user);
				}
			}
		}
	}
	catch( const std::exception & e ) {
		Server::logMsg(ERROR, e.what());
	}
	return true;
}
