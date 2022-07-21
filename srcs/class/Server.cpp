#include <algorithm> // transform
#include <arpa/inet.h>
#include <cerrno> // errno
#include <cstring> // strerror()
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
	_config(),
	_creationTime(),
	_pollfds(),
	_users(),
	_lookupUsers(),
	_lookupChannels(),
	_banList() {}

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
	std::string													line;
	std::string													prefix;
	std::string													cmdName;
	std::string													params;
	std::map<std::string const, t_fct const>::const_iterator	it;

	do
	{
		line = msg.substr(0, msg.find('\n'));
		if (*(line.end() - 1) == '\r')
			line.erase(line.end() - 1);
		if (line[0] == ':')
			prefix = line.substr(1, line.find(' ') - 1);
		cmdName = line.substr(prefix.length(), line.find(' ', prefix.length()));
		if (cmdName.empty())
			continue;
		std::transform<std::string::iterator, std::string::iterator, int (*)(int const)>(cmdName.begin(), cmdName.end(), cmdName.begin(), ::toupper);
		params = line.substr(cmdName.length());
		params.erase(0, params.find_first_not_of(' '));
		params.erase(params.find_last_not_of(' ') + 1);
		it = this->_lookupCmds.find(cmdName);
		if (it == this->_lookupCmds.end())
			Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") " + cmdName + ' ' + params + RED " Unknown" RESET);
		else
		{
			Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") " + cmdName + ' ' + params);
			if (!(this->*it->second)(user, params))
				return false;
		}
		msg.erase(0, msg.find('\n') + 1);
	} while (!msg.empty());
	return true;
}

/**
 * @brief	Write a formated log message to the standard output.
 * 
 * @param	msg The message to write.
 */
void	Server::logMsg(uint const type, std::string const &msg)
{
	char	nowtime[64];
	time_t	rawtime;

	time(&rawtime);
	strftime(nowtime, 64, "%Y/%m/%d %H:%M:%S", localtime(&rawtime));
	std::cout << "[" << nowtime << "][" << this->_lookupLogMsgTypes[type] << "] " << msg << '\n';
}

/**
 * @brief	Add a user in the banlist.
 * 
 * @param	user The user to ban.
 */
void	Server::addToBanList(User const &user)
{
	if (find(this->_banList.begin(), this->_banList.end(), user.getNickname()) == this->_banList.end())
		this->_banList.push_back(user.getNickname());
}

/**
 * @brief	Check every user socket connection, receive messages from
 * 			each of them, and process the received messages.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::recvAll(void)
{
	char						buff[BUFFER_SIZE + 1];
	ssize_t						retRecv;
	std::string					msg;
	std::list<User>::iterator	it;

	if (poll(&_pollfds[0], _pollfds.size(), (TIMEOUT * 1000) / 10) == -1)
	{
		Server::logMsg(ERROR, "poll: " + std::string(strerror(errno)));
		return false;
	}
	for (it = this->_users.begin() ; it != this->_users.end() ; ++it)
	{
		retRecv = recv(it->getSocket(), buff, BUFFER_SIZE, MSG_DONTWAIT);
		while (retRecv > 0)
		{
			buff[retRecv] = 0;
			msg.append(buff);
			if (msg.find("\r\n") != std::string::npos)
				break ;
			retRecv = recv(it->getSocket(), buff, BUFFER_SIZE, MSG_DONTWAIT);
		}
		// if (retRecv == -1)
		// {
		// 	perror("recv");
		// 	return false;
		// }
		if (!retRecv) // Check whith the PING
		{
			Server::logMsg(INTERNAL, "(" + this->toString(it->getSocket()) + ") Connection lost");
			close(it->getSocket());
			this->_lookupUsers.erase(it->getNickname());
			this->_users.erase(it);
		}
		else
		{
			if (!this->judge(*it, msg) || (!it->getMsg().empty() && !this->replySend(*it)))
				return false;
		}
		if (it->getSocket() == -1)
			it = this->_users.erase(it);
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
bool	Server::replyPush(User &user, std::string const &line)
{
	try
	{
		if (!user.getMsg().empty())
			user.setMsg(std::string(user.getMsg()).append("\n"));
		user.setMsg(std::string(user.getMsg()).append(":" + this->_config["server_name"] + " " + line));
	}
	catch (std::exception const &e)
	{
		Server::logMsg(ERROR, "    Exception: " + std::string(e.what()));
		return false;
	}
	return true;
}

/**
 * @brief	Send a reply message to an user client.
 * 
 * @param	user The user to send the reply message to.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::replySend(User &user)
{
	std::string	line;

	user.setMsg(std::string(user.getMsg()).append("\r\n"));
	if (send(user.getSocket(), user.getMsg().c_str(), user.getMsg().size() + 1, 0) == -1)
	{
		Server::logMsg(ERROR, "send: " + std::string(strerror(errno)));
		return false;
	}
	while (!user.getMsg().empty())
	{
		line = user.getMsg().substr(0, user.getMsg().find('\n'));
		user.setMsg(std::string(user.getMsg()).erase(0, user.getMsg().find('\n') + 1));
		if (user.getMsg().empty())
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
	if (newUser != -1)
	{
		this->_users.push_back(User());
		this->_users.back().setAddr(addr);
		this->_users.back().setSocket(newUser);
		this->_lookupUsers.insert(std::pair<std::string, User *const>(this->_users.back().getNickname(), &this->_users.back()));
		this->_pollfds.push_back(pollfd());
		this->_pollfds.back().fd = newUser;
		this->_pollfds.back().events = POLLIN | POLLOUT;
		fcntl(newUser, F_SETFL, O_NONBLOCK | O_DIRECT);
		// hostent *host = gethostbyname(inet_ntoa(addr.sin_addr));
		// this->_users[newUser].setHostname(host->h_name);
		Server::logMsg(INTERNAL, "(" + Server::toString(this->_users.back().getSocket()) + ") Connection established");
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
	char	nowtime[64];
	time_t	rawtime;
	uint	idx;

	this->_config.init("config/default.conf"); // XXX add config.init()
	this->_config["server_password"] = password;
	time(&rawtime);
	strftime(nowtime, 64, "%Y/%m/%d %H:%M:%S", localtime(&rawtime));
	this->_creationTime = nowtime;
	for (idx = 0U ; Server::_arrayCmds[idx].second ; ++idx)
		this->_lookupCmds.insert(Server::_arrayCmds[idx]);
	for (idx = 0U ; Server::_arrayLogMsgTypes[idx].second ; ++idx)
		this->_lookupLogMsgTypes.insert(Server::_arrayLogMsgTypes[idx]);
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
		Server::logMsg(ERROR, "socket: " + std::string(strerror(errno)));
		return false;
	}
	Server::logMsg(INTERNAL, "(" + Server::toString(this->_socket) + ") Socket created");

	optval = 1;
	if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval)))
	{
		Server::logMsg(ERROR, "setsockopt: " + std::string(strerror(errno)));
		this->stop();
		return false;
	}
	Server::logMsg(INTERNAL, "(" + Server::toString(this->_socket) + ") Socket options set");

	addr.sin_addr.s_addr = inet_addr(this->_config["host"].c_str());
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	if (bind(this->_socket, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)))
	{
		Server::logMsg(ERROR, "bind: " + std::string(strerror(errno)));
		this->stop();
		return false;
	}
	Server::logMsg(INTERNAL, "(" + Server::toString(this->_socket) + ") Socket bound");
	if (listen(this->_socket, SOMAXCONN))
	{
		Server::logMsg(ERROR, "listen: " + std::string(strerror(errno)));
		this->stop();
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
	Server::logMsg(INTERNAL, "    Server stopped");
	this->_lookupLogMsgTypes.clear();
	this->_lookupChannels.clear();
	this->_lookupUsers.clear();
	this->_lookupCmds.clear();
	this->_users.clear();
	if (this->_socket >= 0)
		close(this->_socket);
	this->_socket = -1;
	this->_state = STOPPED;
}

// ************************************************************************** //
//                             Private Attributes                             //
// ************************************************************************** //

std::pair<std::string const, Server::t_fct const> const	Server::_arrayCmds[] = {
	std::pair<std::string const, Server::t_fct const>(std::string("DIE"), &Server::DIE),
	std::pair<std::string const, Server::t_fct const>(std::string("JOIN"), &Server::JOIN),
	std::pair<std::string const, Server::t_fct const>(std::string("KICK"), &Server::KICK),
	std::pair<std::string const, Server::t_fct const>(std::string("KILL"), &Server::KILL),
	std::pair<std::string const, Server::t_fct const>(std::string("MODE"), &Server::MODE),
	std::pair<std::string const, Server::t_fct const>(std::string("NICK"), &Server::NICK),
	std::pair<std::string const, Server::t_fct const>(std::string("OPER"), &Server::OPER),
	std::pair<std::string const, Server::t_fct const>(std::string("PART"), &Server::PART),
	std::pair<std::string const, Server::t_fct const>(std::string("PASS"), &Server::PASS),
	std::pair<std::string const, Server::t_fct const>(std::string("PING"), &Server::PING),
	std::pair<std::string const, Server::t_fct const>(std::string("PRIVMSG"), &Server::PRIVMSG),
	std::pair<std::string const, Server::t_fct const>(std::string("QUIT"), &Server::QUIT),
	std::pair<std::string const, Server::t_fct const>(std::string("USER"), &Server::USER),
	std::pair<std::string const, Server::t_fct const>(std::string(), NULL)
};

std::pair<uint const, char const *const> const	Server::_arrayLogMsgTypes[] = {
	std::pair<uint const, char const *const>(ERROR, RED " Errors " RESET),
	std::pair<uint const, char const *const>(INTERNAL, WHITE "Internal" RESET),
	std::pair<uint const, char const *const>(RECEIVED, GREEN "Received" RESET),
	std::pair<uint const, char const *const>(SENT, MAGENTA "  Sent  " RESET),
	std::pair<uint const, char const *const>(0, NULL),
};
