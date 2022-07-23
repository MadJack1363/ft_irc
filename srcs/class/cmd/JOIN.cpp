#include "class/Server.hpp"

/**
 * @brief Config the message to send form CMD Join
 * 
 * @param user the people to send the reply
 * @param channel_name the name of the channel he join
 * @return void
 */
void	Server::joinSend(User &user, std::string &channel_name, std::string const &name_join)
{
	// FIXME with the list of user inside
	// FIXME with the main channel of irssi message appear
	std::string tmp = ":" + user.getNickname() + "!" + user.getUsername() + "@" + this->_config["host"] + " IP JOIN :" +channel_name;
	user.setMsg(tmp);
	this->replyPush(user, "332 " + user.getNickname() + " " + channel_name + " :" + name_join + " has joined the channel");
	this->replyPush(user, "353 " + user.getNickname() + " = " + channel_name + " :" + user.getNickname());// TODO add all user in this channel
	this->replyPush(user, "366 " + user.getNickname() + " " + channel_name + " :End of /NAMES list");
	return ;
}

/**
 * 
 * @brief	Make an user joining a channel.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::JOIN(User &user, std::string &params)
{
	// MEMO check for 0 https://datatracker.ietf.org/doc/html/rfc2812#section-3.2.1
	std::vector<std::string>	channel_join;

	params = params.c_str() + params.find('#') + 1;
	while (params.find('#') != std::string::npos)
	{
		channel_join.push_back(params.substr(0, params.find(',')));
		params = params.c_str() + params.find('#') + 1;
	}
	channel_join.push_back(params.substr(0, params.length()));
	if (channel_join.empty())
	{
		return this->replyPush(user, "461 JOIN :Not enough parameters");
	}
	for (std::vector<std::string>::iterator ite = channel_join.begin(); ite != channel_join.end(); ite++)
	{
		Channel &chan = this->_lookupChannels[*ite];
		if (!user.getMsg().empty())
			if (!this->replySend(user))
				return false;
		if (chan.getName().compare("Empty") == 0){
			chan.setName(*ite);
		}
		chan.addUser(user);
		std::string tmp = "#" + *ite;
		for (std::vector<User *>::iterator itv = chan.getUsers().begin(); itv != chan.getUsers().end(); itv++)
		{
			if ((*itv)->getNickname().compare(user.getNickname()) == 0)
				joinSend(*(*itv), tmp, user.getNickname());
			else
			{
				std::string tmp2 = ":" + user.getNickname() + "!" + user.getUsername() + "@" + this->_config["host"] + " IP JOIN :" + tmp;
				(*itv)->setMsg(tmp);
				this->replyPush(*(*itv), "332 " + (*itv)->getNickname() + " " + tmp + " :" + user.getNickname() + " has joined the channel");
			}
			Server::replySend(*(*itv));
		}
	}
	return true;
}
