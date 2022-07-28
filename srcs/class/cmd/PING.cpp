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
	if (params.empty())
		return this->replyPush(user, "461 " + user.getNickname() + " PING :Not enough parameters");
	return this->replyPush(user, "PONG " + params);
}

// TODO: Write the function comment.
bool Server::checkStillAlive(User &user)
{
	time_t		clock[2];
	ssize_t		retRecv;
	char		buff[BUFFER_SIZE + 1];
	std::string	msg;

	time(&clock[START]);
	if (!this->replyPush(user, "PING " + user.getNickname()) ||
		!this->replySend(user))
		return false;
	while (1)
	{
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
			return false ;
		}
	}
	std::string line = msg.substr(0, msg.find('\n'));
	if (*(line.end() - 1) == '\r')
		line.erase(line.end() - 1);
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") " + line);
	line.erase(0, line.find_first_of(':') + 1);
	if (line.compare(user.getNickname()) != 0)
		return false;
	return true;
}
