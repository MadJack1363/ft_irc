#include "class/Server.hpp"

/**
 * @brief	Disconnect an user from the server.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::QUIT(User &user, std::string &params)
{
	std::string												channels;
	std::string												reason;
	std::string												subParams;
	std::map<std::string const, Channel *>::const_iterator	cit;

	if (!user.getChannels().empty())
	{
		for (cit = user.getChannels().begin() ; cit != user.getChannels().end() ; ++cit)
			channels += cit->first + ',';
		channels.erase(channels.end() - 1);

		reason = params;
		if (*reason.begin() == ':')
			reason.erase(reason.begin());
		reason = "Quit: " + reason;

		subParams = channels + " :" + reason;
		if (this->PART(user, subParams))
			return false;
	}
	close(user.getSocket());
	user.setSocket(-1);
	return true;
}
