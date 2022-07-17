#include "class/Server.hpp"

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

	params = params.c_str() + params.find(':') + 1;
	while (params.find(',') != std::string::npos)
	{
		channel_left.push_back(params.substr(0, params.find(',')));
		params = params.c_str() + params.find(',') + 1;
	}
	channel_left.push_back(params.substr(0, params.find(' ')));
	params = params.c_str() + params.find(' ');
	if (params.length() > 1)
		left_message = params;
	try {
		for(std::vector<std::string>::iterator ite = channel_left.begin();ite != channel_left.end();ite++)
		{
			Channel	&myChan = this->_channels[*ite];
			if (myChan.getUsers().size() == 1)
			{
				this->_channels.erase(*ite);
			}
			else
			{
				myChan;
				myChan.getUsers().erase(std::find(myChan.getUsers().begin(), myChan.getUsers().end(), const_cast<User *>(&user)));
				std::string	cpy = *ite + left_message;
				PRIVMSG(user, cpy);
			}
		}
	}
	catch( const std::exception & e ) {
		Server::logMsg(ERROR, e.what());
	}
	return true;
}
