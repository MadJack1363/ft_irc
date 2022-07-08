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
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") QUIT " + params);
	this->_users.erase(user.getSocket());
	this->_state = STOPPED; // XXX To be removed, temporary solution to end properly the server.
	return true;
}
