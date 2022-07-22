#include "class/Server.hpp"

bool	Server::joinSend(User &user, std::string &channel_name)
{
	//FIXME In hostname is not localhost
	std::string tmp = ":" + user.getNickname() + "!" + user.getNickname() + "@" + this->_config["host"] + " IP JOIN :" +channel_name;
	user.setMsg(tmp);
	// if (!replySend(user))
	// 	return false;
	// FIXME Check for NO TOPIC if its the first user
	this->replyPush(user, "332 " + user.getNickname() + " " + channel_name + " :" + user.getNickname() + " has joinned the channel");
	this->replyPush(user, "353 " + channel_name);
	return true;
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
		if (!user.getMsg().empty())
			if (!this->replySend(user))
				return false;
		// TODO Banned from channel
		// return this->replyPush(user, "474 " + "*ite" + ":Cannot join channel (+b)");
		if (this->_lookupChannels[*ite].getName().compare("Empty") == 0){
			this->_lookupChannels[*ite].setName(*ite);
		}
		this->_lookupChannels[*ite].addUser(user);
		std::string tmp = "#" + *ite;
		joinSend(user, tmp);
	}
	return true;
}
