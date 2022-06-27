#include <arpa/inet.h>
#include <cstdio> // perror()
#include <cstdlib> // abs()
#include <ctime> // time(), localtime(), strftime()
#include <sstream>
#include "class/Server.hpp"

#define PING 10

// ************************************************************************** //
//                                Constructors                                //
// ************************************************************************** //

Server::Server(void) :
	_state(STOPPED),
	_socket(-1),
	_name(),
	_password(),
	_pollfds(),
	_users(),
	_channels(),
	_cmds() {}

// ************************************************************************* //
//                                Destructors                                //
// ************************************************************************* //

Server::~Server(void) {}

// ************************************************************************** //
//                          Private Member Functions                          //
// ************************************************************************** //

/**
 * @brief	Terminate the IRC server.
 * 			This command is reserved for IRC operators.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdDie(User &user, std::string const &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") DIE " + params);
	if (user.getIsOperator())
	{
		Server::logMsg(SENT, "DIE " + params + ", To " + user.getNickname());
		this->_state = STOPPED;
		return true;
	}
	else
	{
		Server::logMsg(SENT, "DIE " + params + ", To " + user.getNickname() + "; ERROR: Not an operator");
		return false;
	}
	return true;
}

/**
 * @brief	Make an user joining a channel.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdJoin(User &user, std::string const &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") JOIN " + params);
	this->_channels.insert(std::make_pair<std::string, Channel>(params, Channel()));
	this->_channels[params].addUser(user);
	return true;
}

/**
 * @brief	Kick an user from a channel.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdKick(User &user, std::string const &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") KICK " + params);
	return true;
}

/**
 * @brief	Remove an user from the network.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdKill(User &user, std::string const &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") KILL " + params);
	return true;
}

/**
 * @brief	Send a message either to a channel or to an user.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdMsg(User &user, std::string const &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") MSG " + params);
	return true;
}

/**
 * @brief	Set a new nickname for an user.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdNick(User &user, std::string const &params)
{
	std::map<int, User>::iterator	it;

	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") NICK " + params);
	if (params.empty())
		return this->reply(user, "431 " + user.getNickname() + " :No nickname given");
	for (it = this->_users.begin() ;
		it != this->_users.end() && it->second.getNickname() != params ;
		++it);
	if (it != this->_users.end())
		return this->reply(user, "433 " + user.getNickname() + " :Nickname already in use");
	user.setNickname(params);
	return this->reply(user, "NICK " + user.getNickname());
}

/**
 * @brief	Make an user being promoted to operator status.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdOper(User &user, std::string const &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") OPER " + params);
	return true;
}

/**
 * @brief	Make an user leaving a channel.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdPart(User &user, std::string const &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") PART " + params);
	return true;
}

/**
 * @brief	Check if a provided password is correct to connect to the server.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdPass(User &user, std::string const &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") PASS " + params);

	// if (this->_password.empty())
	// 	return true;
	// if (params == this->_password)
	// 	return true;
	return true;
}

/**
 * @brief 
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdPing(User &user, std::string const &params)
{
	std::string	reply;

	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") Command PING " + params);
	return this->reply(user, "PING " + user.getNickname());
}

/**
 * @brief	Disconnect an user from the server.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdQuit(User &user, std::string const &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") Command QUIT " + params);

	this->_state = STOPPED; // XXX To be removed, temporary solution to end properly the server.
	return true;
}

/**
 * @brief	Update data of an user.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdSet(User &user, std::string const &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") Command SET " + params);
	return true;
}

/**
 * @brief	Set a new username, hostname and realname for an user.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdUser(User &user, std::string const &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") Command USER " + params);
	return true;
}

/**
 * @brief	Determine what to do depending on the given `msg`.
 * 
 * @param	user The user that sent the message.
 * @param	msg The raw message received from a client.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::judge(User &user, std::string &msg)
{
	std::string	commandName;
	std::string	params;
	std::string	line;
	t_fct		command;

	do
	{
		line = msg.substr(0, msg.find('\n') - 1);
		commandName = line.substr(0, line.find(' '));
		params = line.substr(commandName.length());
		params.erase(0, params.find_first_not_of(' '));

		command = this->_cmds[commandName];
		if (!command)
		{
			if (!commandName.empty())
				Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") Command " + commandName + RED " Unknown" RESET);
		}
		else if (!(this->*command)(user, params))
			return false;
		msg.erase(0, msg.find('\n') + 1);
	} while (!line.empty());
	return true;
}

/**
 * @brief	Write a formated log message to the standard output.
 * 
 * @param	msg The message to write.
 */
void	Server::logMsg(enum e_logMsg const type, std::string const &msg)
{
	char	logtime[64];
	time_t	rawtime;
	int		idx;

	for (idx = 0 ; idx < 3 && _lookupLogMsgTypes[idx].first != type ; ++idx);
	time(&rawtime);
	strftime(logtime, 64, "%Y/%m/%d %H:%M:%S", localtime(&rawtime));
	std::cout
	<< "["
	<< logtime
	<< "]["
	<< _lookupLogMsgTypes[idx].second
	<< "] "
	<< msg
	<< '\n';
}

/**
 * @brief	Check every user socket connection, receive messages from
 * 			each of them, and process the received messages.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::recvAll(void)
{
	std::map<int, User>::iterator	it;
	std::string						message;
	char							buff[BUFFER_SIZE];
	ssize_t							retRecv;

	if (poll(&_pollfds[0], _pollfds.size(), (PING * 1000) / 10) == -1)
	{
		perror("poll");
		return false;
	}
	for (it = this->_users.begin() ; it != this->_users.end() ; ++it)
	{
		retRecv = recv(it->second.getSocket(), buff, BUFFER_SIZE, 0);
		while (retRecv > 0)
		{
			message.append(buff);
			if (message.find("\r\n") != std::string::npos)
				break ;
			retRecv = recv(it->second.getSocket(), buff, BUFFER_SIZE, 0);
		}
		if (retRecv == -1)
		{
			perror("recv");
			return false;
		}
		if (!retRecv)
		{
			Server::logMsg(INTERNAL, "(" + this->toString(it->second.getSocket()) + ") Connection lost");
			this->_users.erase(it);
			return true;
		}
		if (!this->judge(it->second, message))
			return false;
		message.clear();
	}
	return true;
}

/**
 * @brief	Send a reply message to an user client.
 * 
 * @param	user The user to send the reply message to.
 * @param	msg The message to send.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::reply(User const &user, std::string const &msg) const
{
	std::string	toSend(":" + this->_name + " " + msg + "\r\n");

	if (send(user.getSocket(), toSend.c_str(), toSend.size() + 1, 0) == -1)
	{
		perror("send");
		return false;
	}
	Server::logMsg(SENT, "(" + Server::toString(user.getSocket()) + ") " + msg);
	return true;
}

/**
 * @brief	Convert an int to a string.
 * 
 * @param	nb The int to convert.
 * 
 * @return	The string representation of the int.
 */
std::string	Server::toString(int const nb)
{
	std::stringstream	ss;

	ss << nb;
	return ss.str();
}

/**
 * @brief	Accept the new clients connection and create users for each one.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::welcomeDwarves(void)
{
	sockaddr_in	addr = {};
	socklen_t	addrlen = sizeof(addr);
	int			newUser;

	newUser = accept(this->_socket, reinterpret_cast<sockaddr *>(&addr), &addrlen);
	if (newUser == -1)
	{
		// perror("accept");
	}
	else
	{
		this->_pollfds.push_back(pollfd());
		this->_pollfds.back().fd = newUser;
		this->_pollfds.back().events = POLLIN | POLLOUT;
		this->_users.insert(std::make_pair<int, User>(newUser, User()));
		this->_users[newUser].setSocket(newUser);
		Server::logMsg(INTERNAL, "(" + this->toString(newUser) + ") Connection established");
		this->reply(this->_users[newUser], "001 "+ this->_users[newUser].getNickname() +" :Welcome to the Mine");
	}
	return true;
}

// ************************************************************************* //
//                          Public Member Functions                          //
// ************************************************************************* //

/**
 * @brief	Configure the internal attributes of the server.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::init(std::string const password)
{
	int													idx;
	std::map<std::string const, t_fct const>::iterator	it;

	this->_name = "ircserv";
	this->_password = password;
	for (idx = 0 ; Server::_lookupCmds[idx].second ; ++idx)
		this->_cmds.insert(Server::_lookupCmds[idx]);
	return true;
}

/**
 * @brief	Main routine of the server, consisting of accepting new connections,
 * 			receiving messages from clients, and processing them.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::run(void)
{
	while (this->_state == RUNNING)
	{
		if (!this->welcomeDwarves() ||
			!this->recvAll())
		{
			this->stop();
			return false;
		}
	}
	return true;
}

/**
 * @brief	Start the server, configuring the socket and the port for listening.
 * 
 * @param	port The port to listen on.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::start(uint16_t const port)
{
	int			optval;
	sockaddr_in	addr;

	this->_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (this->_socket == -1)
	{
		perror("socket");
		return false;
	}
	Server::logMsg(INTERNAL, "(" + Server::toString(this->_socket) + ") Socket created");

	optval = 1;
	if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval)))
	{
		this->stop();
		perror("setsockopt");
		return false;
	}
	Server::logMsg(INTERNAL, "(" + Server::toString(this->_socket) + ") Socket options set");

	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // ip hard code for now
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	if (bind(this->_socket, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)))
	{
		this->stop();
		perror("bind");
		return false;
	}
	if (listen(this->_socket, SOMAXCONN))
	{
		this->stop();
		perror("listen");
		return false;
	}
	_pollfds.push_back(pollfd());
	_pollfds.back().fd = this->_socket;
	_pollfds.back().events = POLLIN;
	this->_state = RUNNING;
	return true;
}

/**
 * @brief	Disconnect every users, release the resources of the server,
 * 			and stop it.
 */
void	Server::stop(void)
{
	this->_users.clear();
	this->_channels.clear();
	this->_cmds.clear();
	if (this->_socket >= 0)
		close(this->_socket);
	this->_socket = -1;
	this->_state = STOPPED;
	Server::logMsg(INTERNAL, "    Server stopped");
}

// ************************************************************************** //
//                             Private Attributes                             //
// ************************************************************************** //

std::pair<std::string const, t_fct const> const	Server::_lookupCmds[] = {
	std::make_pair<std::string const, t_fct const>(std::string("DIE"), &Server::cmdDie),
	std::make_pair<std::string const, t_fct const>(std::string("JOIN"), &Server::cmdJoin),
	std::make_pair<std::string const, t_fct const>(std::string("KICK"), &Server::cmdKick),
	std::make_pair<std::string const, t_fct const>(std::string("KILL"), &Server::cmdKill),
	std::make_pair<std::string const, t_fct const>(std::string("MSG"), &Server::cmdMsg),
	std::make_pair<std::string const, t_fct const>(std::string("NICK"), &Server::cmdNick),
	std::make_pair<std::string const, t_fct const>(std::string("OPER"), &Server::cmdOper),
	std::make_pair<std::string const, t_fct const>(std::string("PART"), &Server::cmdPart),
	std::make_pair<std::string const, t_fct const>(std::string("PASS"), &Server::cmdPass),
	std::make_pair<std::string const, t_fct const>(std::string("PING"), &Server::cmdPing),
	std::make_pair<std::string const, t_fct const>(std::string("QUIT"), &Server::cmdQuit),
	std::make_pair<std::string const, t_fct const>(std::string("SET"), &Server::cmdSet),
	std::make_pair<std::string const, t_fct const>(std::string("USER"), &Server::cmdUser),
	std::make_pair<std::string const, t_fct const>(std::string(""), NULL),
};

std::pair<enum e_logMsg const, char const *> const	Server::_lookupLogMsgTypes[] = {
	std::make_pair<enum e_logMsg const, char const *>(INTERNAL, WHITE "Internal" RESET),
	std::make_pair<enum e_logMsg const, char const *>(RECEIVED, GREEN "Received" RESET),
	std::make_pair<enum e_logMsg const, char const *>(SENT, MAGENTA "  Sent  " RESET),
};
