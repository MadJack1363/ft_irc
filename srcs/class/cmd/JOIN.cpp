#include "class/Server.hpp"

/**
 * @brief Config the message to send form CMD Join
 * 
 * @param user the people to send the reply
 * @param channel the channel of the user join
 * @param name_join the name of the people join the channels
 * @return void
 */
void	Server::joinSend(User &user, Channel &channel, std::string const &name_join)
{
	std::string tmp = ":" + user.getNickname() + "!" + user.getUsername() + "@" + this->_config["host"] + " JOIN :#" +channel.getName();
	user.setMsg(tmp);
	tmp.clear();
	this->replyPush(user, ':' + user.getMask() + " 332 " + user.getNickname() + " #" + channel.getName() + " :" + name_join + " has joined the channel");
	for (std::vector<User *>::const_iterator itv = channel.getUsers().begin(); itv != channel.getUsers().end();itv++)
		tmp += " " + (*itv)->getNickname();
	this->replyPush(user, ':' + user.getMask() + " 353 " + user.getNickname() + " = #" + channel.getName() + " :" + tmp);
	this->replyPush(user, ':' + user.getMask() + " 366 " + user.getNickname() + " #" + channel.getName() + " :End of /NAMES list");
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
	// TODO Change the function for use iterator for all the fonctionnement
	// std::vector<std::string>	channelToJoin; #test,#test1,#test2

	std::string					subParams;
	std::string					channelJoin;
	std::string::const_iterator	cit0 = params.begin();
	std::string::const_iterator	cit1 = params.begin();


	for (;cit1 != ":";cit1++);
	channelJoin = std::string(cit0, cit1);
	

	// channelJoin = params.str(params.begin() , params.find(':'));

	// if (channelToJoin.empty())
	// {
	// 	return this->replyPush(user, ':' + user.getMask() + "461 JOIN :Not enough parameters");
	// }
	// if (channelToJoin.find("0") != channelToJoin.end())// TODO check for discussion FOR THE error or NO
	// 		return this->PART(user, *channelToJoin.find("0"));

	// for (std::vector<std::string>::iterator ite = channelToJoin.begin(); ite != channelToJoin.end(); ite++)
	// {
	// 	Channel &chan = this->_lookupChannels[*ite];
	// 	if (!user.getMsg().empty())
	// 		if (!this->replySend(user))
	// 			return false;
	// 	if (chan.getName().compare("Empty") == 0){
	// 		chan.setName(*ite);
	// 	}
	// 	chan.addUser(user);
	// 	std::string tmp = "#" + *ite;
	// 	for (std::vector<User *>::const_iterator itv = chan.getUsers().begin(); itv != chan.getUsers().end(); itv++)
	// 	{
	// 		if ((*itv)->getNickname().compare(user.getNickname()) == 0)
	// 			joinSend(*(*itv), chan, user.getNickname());
	// 		else
	// 		{
	// 			std::string tmp2 = ":" + user.getNickname() + "!" + user.getUsername() + "@" + this->_config["host"] + " IP JOIN :" + tmp;
	// 			(*itv)->setMsg(tmp);
	// 			this->replyPush(*(*itv), ':' + user.getMask() + "332 " + (*itv)->getNickname() + " " + tmp + " :" + user.getNickname() + " has joined the channel");
	// 		}
	// 		Server::replySend(*(*itv));
	// 	}
	// }
	return true;
}
