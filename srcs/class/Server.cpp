#include <arpa/inet.h>
#include <cstdio> // perror()
#include <cstdlib> // abs()
#include <netdb.h> // gethostbyname()
#include <sstream>
#include <string>
#include "class/Server.hpp"
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define TIMEOUT 10

// ************************************************************************** //
//                                Constructors                                //
// ************************************************************************** //

Server::Server(void) :
	_state(STOPPED),
	_socket(-1),
	_ip("127.0.0.1"),
	_msg(),
	_name("Khazad-Dum"),
	_version("1.0"),
	_password(),
	_creationTime("a long time ago, in a galaxy far, far away..."),
	_availableUserModes("o"),
	_availableChannelModes(),
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
 * @brief	Determine what to do depending on the given `msg`.
 * 
 * @param	user The user that sent the message.
 * @param	msg The raw message received from a client.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::judge(User &user, std::string &msg)
{
	std::string	line;
	std::string	prefix;
	std::string	cmdName;
	std::string	params;
	t_fct		cmdCall;

	do
	{
		line = msg.substr(0, msg.find('\n'));
		if (*(line.end() - 1) == '\r')
			line.erase(line.end() - 1);
		if (line[0] == ':')
			prefix = line.substr(1, line.find(' ') - 1);
		cmdName = line.substr(prefix.length(), line.find(' ', prefix.length()));
		params = line.substr(cmdName.length());
		params.erase(0, params.find_first_not_of(' '));
		params.erase(params.find_last_not_of(' ') + 1);
		cmdCall = this->_cmds[cmdName];
		if (!cmdCall)
		{
			if (!cmdName.empty())
			{
				Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") Command " + cmdName + RED " Unknown" RESET);
			}
		}
		else if (!(this->*cmdCall)(user, params))
			return false;
		msg.erase(0, msg.find('\n') + 1);
	} while (!msg.empty());
	return true;
}

/**
 * @brief	Write a formated log message to the standard output.
 * 
 * @param	msg The message to write.
 */
void	Server::logMsg(enum e_logMsg const type, std::string const &msg)
{
	char	nowtime[64];
	time_t	rawtime;
	int		idx;

	for (idx = 0 ; idx < 3 && _lookupLogMsgTypes[idx].first != type ; ++idx);
	time(&rawtime);
	strftime(nowtime, 64, "%Y/%m/%d %H:%M:%S", localtime(&rawtime));
	std::cout
	<< "["
	<< nowtime
	<< "]["
	<< _lookupLogMsgTypes[idx].second
	<< "] "
	<< msg
	<< '\n';
}

/**
 * @brief	Write all information of the user in the log.
 * 
 * @param	user The user to write.
 */
void	Server::printUser(User const &user)
{
	Server::logMsg(INTERNAL, "User : ");
	Server::logMsg(INTERNAL, "\tSocket : " + Server::toString(user.getSocket()));
	Server::logMsg(INTERNAL, "\tNickname : " + user.getNickname());
	Server::logMsg(INTERNAL, "\tHostname : " + user.getHostname());
	Server::logMsg(INTERNAL, "\tRealname : " + user.getRealname());
	Server::logMsg(INTERNAL, "\tPassword : " + user.getPassword());
}

/**
 * @brief	Check every user socket connection, receive messages from
 * 			each of them, and process the received messages.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::recvAll(void)
{
	size_t const					originalSize = this->_users.size();
	char							buff[BUFFER_SIZE + 1];
	ssize_t							retRecv;
	std::string						msg;
	std::map<int, User>::iterator	it;

	if (poll(&_pollfds[0], _pollfds.size(), (TIMEOUT * 1000) / 10) == -1)
	{
		perror("poll");
		return false;
	}
	for (it = this->_users.begin() ; it != this->_users.end() ; ++it)
	{
		retRecv = recv(it->second.getSocket(), buff, BUFFER_SIZE, 0);
		// retRecv = recv(it->second.getSocket(), buff, BUFFER_SIZE, MSG_DONTWAIT);// mettre celle la quand on aura fais fcntl
		while (retRecv > 0)
		{
			buff[retRecv] = 0;
			msg.append(buff);
			if (msg.find("\r\n") != std::string::npos)
				break ;
			retRecv = recv(it->second.getSocket(), buff, BUFFER_SIZE, 0);
		}
		if (retRecv == -1)
		{
			perror("recv");
			return false;
		}
		if (!retRecv) // Check whith the PING
		{
			Server::logMsg(INTERNAL, "(" + this->toString(it->second.getSocket()) + ") Connection lost");
			this->_users.erase(std::map<int, User>::iterator(it));
		}
		else if (!this->judge(it->second, msg)
			|| (!this->_msg.empty() && !this->replySend(it->second)))
			return false;
		if (originalSize != this->_users.size())
			break ;
		msg.clear();
	}
	return true;
}

/**
 * @brief	Append a line to the message to send to an user client.
 * 
 * @param	line The line to append.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::replyPush(std::string const &line)
{
	if (!this->_msg.empty())
		this->_msg.append("\n");
	this->_msg.append(":" + this->_name + " " + line);
	return true;
}

/**
 * @brief	Send a reply message to an user client.
 * 
 * @param	user The user to send the reply message to.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::replySend(User const &user)
{
	std::string	line;

	this->_msg.append("\r\n");
	if (send(user.getSocket(), this->_msg.c_str(), this->_msg.size() + 1, 0) == -1)
	{
		perror("send");
		return false;
	}
	while (!this->_msg.empty())
	{
		line = this->_msg.substr(0, this->_msg.find('\n'));
		this->_msg.erase(0, this->_msg.find('\n') + 1);
		if (_msg.empty())
			line.erase(line.end() - 1);
		Server::logMsg(SENT, "(" + Server::toString(user.getSocket()) + ") " + line);
	}
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
		// fcntl(newUser, F_SETFL, O_NONBLOCK);
		// std::string hostaddr = inet_ntoa(addr.sin_addr);
		// char hostname[NI_MAXHOST];
		// if (getnameinfo((struct sockaddr *)&address, sizeof(address), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
		// 	error("getnameinfo", false);
		// else
		// 	this->hostname = hostname;
		// getHostbyname
		hostent *host = gethostbyname(inet_ntoa(addr.sin_addr));
		this->_users[newUser].setHostname(host->h_name);
		this->_users[newUser].setSocket(newUser);
		this->_users[newUser].setAddr(addr);
		Server::logMsg(INTERNAL, "(" + this->toString(newUser) + ") Connection established");
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
	char												nowtime[64];
	time_t												rawtime;
	int													idx;
	std::map<std::string const, t_fct const>::iterator	it;

	this->_password = password;
	time(&rawtime);
	strftime(nowtime, 64, "%Y/%m/%d %H:%M:%S", localtime(&rawtime));
	this->_creationTime = nowtime;
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
	static struct timespec			t0 = {0, 50000};
	static struct timespec			t1 = {0, 0};
	std::map<int, User>::iterator	it;

	while (this->_state == RUNNING)
	{
		if (!this->welcomeDwarves() ||
			!this->recvAll() ||
			nanosleep(&t0, &t1))
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

	addr.sin_addr.s_addr = inet_addr(this->_ip.c_str());
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	if (bind(this->_socket, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)))
	{
		this->stop();
		perror("bind");
		return false;
	}
	Server::logMsg(INTERNAL, "(" + Server::toString(this->_socket) + ") Socket bound");
	if (listen(this->_socket, SOMAXCONN))
	{
		this->stop();
		perror("listen");
		return false;
	}
	Server::logMsg(INTERNAL, "(" + Server::toString(this->_socket) + ") Socket listening");
	_pollfds.push_back(pollfd());
	_pollfds.back().fd = this->_socket;
	_pollfds.back().events = POLLIN | POLLOUT;
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
	std::make_pair<std::string const, t_fct const>(std::string("DIE"), &Server::DIE),
	std::make_pair<std::string const, t_fct const>(std::string("JOIN"), &Server::JOIN),
	std::make_pair<std::string const, t_fct const>(std::string("KICK"), &Server::KICK),
	std::make_pair<std::string const, t_fct const>(std::string("KILL"), &Server::KILL),
	std::make_pair<std::string const, t_fct const>(std::string("MODE"), &Server::MODE),
	std::make_pair<std::string const, t_fct const>(std::string("NICK"), &Server::NICK),
	std::make_pair<std::string const, t_fct const>(std::string("OPER"), &Server::OPER),
	std::make_pair<std::string const, t_fct const>(std::string("PART"), &Server::PART),
	std::make_pair<std::string const, t_fct const>(std::string("PASS"), &Server::PASS),
	std::make_pair<std::string const, t_fct const>(std::string("PING"), &Server::PING),
	std::make_pair<std::string const, t_fct const>(std::string("PRIVMSG"), &Server::PRIVMSG),
	std::make_pair<std::string const, t_fct const>(std::string("QUIT"), &Server::QUIT),
	std::make_pair<std::string const, t_fct const>(std::string("SET"), &Server::SET),
	std::make_pair<std::string const, t_fct const>(std::string("USER"), &Server::USER),
	std::make_pair<std::string const, t_fct const>(std::string(""), NULL),
};

std::pair<enum e_logMsg const, char const *> const	Server::_lookupLogMsgTypes[] = {
	std::make_pair<enum e_logMsg const, char const *>(INTERNAL, WHITE "Internal" RESET),
	std::make_pair<enum e_logMsg const, char const *>(RECEIVED, GREEN "Received" RESET),
	std::make_pair<enum e_logMsg const, char const *>(SENT, MAGENTA "  Sent  " RESET)
};
