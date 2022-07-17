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
	close(user.getSocket());
	user.setSocket(-1);
	this->_finder.erase(user.getNickname());
	return true;
}
