#include "class/Server.hpp"

# define START 0
# define LIMIT 1

/**
 * @brief	Ask the server if the connection is still alive.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::PING(User &user, std::string &params)
{
	return this->replyPush(user, ':' + user.getMask() + " PING " + params)
		&& this->replyPush(user, ':' + user.getMask() + " PONG " + params);
}

bool Server::IDK(User &user)
{
	time_t		clock[2];
	ssize_t		retRecv;
	char		buff[BUFFER_SIZE + 1];
	std::string	msg;

	time(&clock[START]);
	this->replyPush(user, "PING " + user.getNickname());
	this->replySend(user);
	while (1)
	{
		// MEMO a 2 doight de me demander si le mieux nest pas de le mettre en 0 pour les flag de recv
		retRecv = recv(user.getSocket(), buff, BUFFER_SIZE, MSG_DONTWAIT);
		while (retRecv > 0)
		{
			buff[retRecv] = 0;
			msg.append(buff);
			if (msg.find("\r\n") != std::string::npos)
				break ;
			retRecv = recv(user.getSocket(), buff, BUFFER_SIZE, MSG_DONTWAIT);
		}
		if (msg.size() > 0)
			break ;
		time(&clock[LIMIT]);
		if (clock[LIMIT] - clock[START] >= std::strtol(this->_config["timeout"].c_str(), NULL, 10))
		{
			Server::logMsg(ERROR, "Error time " + Server::toString(clock[LIMIT] - clock[START]));
			msg.clear();
			// Server::QUIT(user, msg);
			return false ;
		}
	}
	if (msg.compare(user.getNickname()) != 0)
	return false;
	Server::logMsg(INTERNAL, "PING RECOIS " + msg);
	return true;
	// return this->replyPush(user, "PONG " + params);
}