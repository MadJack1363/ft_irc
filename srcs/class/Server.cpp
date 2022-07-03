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

#define PING 10

// ************************************************************************** //
//                                Constructors                                //
// ************************************************************************** //

Server::Server(void) :
	_state(STOPPED),
	_socket(-1),
	_name("ircserv"),
	_version("1.0"),
	_password(),
	_creationTime(),
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
bool	Server::cmdDie(User &user, std::string &params)
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
bool	Server::cmdJoin(User &user, std::string &params)
{

	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") JOIN " + params);
	if (this->_channels.count(params) == 0){
		params = params.c_str() + params.find('#') + 1;
		this->_channels.insert(std::make_pair<std::string, Channel>(params, Channel(params)));
	}
	this->_channels[params].addUser(user);

	// Channel &tm = this->_channels[params];

	// Server::logMsg(INTERNAL, "Utilisateur log in " + params);
	// for (std::vector<User *>::const_iterator ite = tm.getUsers().begin(); ite != tm.getUsers().end(); ite++)
	// {
	// 	Server::logMsg(INTERNAL, "\t" + (*ite)->getNickname());
	// }
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
bool	Server::cmdKick(User &user, std::string &params)
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
bool	Server::cmdKill(User &user, std::string &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") KILL " + params);
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
bool	Server::cmdMode(User &user, std::string &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") MODE " + params);
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
bool	Server::cmdNick(User &user, std::string &params)
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
bool	Server::cmdOper(User &user, std::string &params)
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
bool	Server::cmdPart(User &user, std::string &params)
{
	std::vector<std::string>		channel_left;
	// std::vector<User *>::iterator	tmp2;

	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") PART " + params);
	params = params.c_str() + params.find(':') + 1;
	while (params.find(',') != std::string::npos)
	{
		channel_left.push_back(params.substr(0, params.find(',')));
		params = params.c_str() + params.find(',') + 1;
	}
	channel_left.push_back(params.substr(0, params.find(',')));
	for(std::vector<std::string>::iterator ite = channel_left.begin();ite != channel_left.end();ite++)
	{
		// config for send a custom message or no of all user of any channel
		if (this->_channels[*ite].getUsers().size() == 1)
		{
			this->_channels.erase(*ite);
		}
		else
		{
			this->_channels[*ite].delUser(user);
			// this.cmdPrivMsg()// need to send message to other user
		}
	}
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
bool	Server::cmdPass(User &user, std::string &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") PASS " + params);
	if (user.getIsRegistered())
		return this->reply(user, "462 :You may not register");
	if (params.empty())
		return this->reply(user, "461 PASS :not enough parameters");
	user.setPassword(params);
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
bool	Server::cmdPing(User &user, std::string &params)
{
	std::string	reply;

	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") PING " + params);
	return this->reply(user, "PING " + user.getNickname());
}

/**
 * @brief	Send a message either to a channel or to an user.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdPrivMsg(User &user, std::string &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") PRIVMSG " + params);
	return true;
}

/**
 * @brief	Disconnect an user from the server.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdQuit(User &user, std::string &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") QUIT " + params);
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
bool	Server::cmdSet(User &user, std::string &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") SET " + params);
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
bool	Server::cmdUser(User &user, std::string &params)
{
	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") USER " + params);
	if (user.getIsRegistered())
		return this->reply(user, "462 :You may not register");
	if (params.empty())
		return this->reply(user, "461 USER :Not enough parameters");
	user.setUsername(params.substr(0, params.find(' ')));
	params.erase(0, params.find(' ') + 1).erase(0, params.find_first_not_of(' '));
	if (params.empty())
		return this->reply(user, "461 USER :Not enough parameters");
	
	user.setHostname(params.substr(0, params.find(' ')));
	params.erase(0, params.find(' ') + 1).erase(0, params.find_first_not_of(' '));
	if (params.empty())
		return this->reply(user, "461 USER :Not enough parameters");
	params.erase(0, params.find(' ') + 1).erase(0, params.find_first_not_of(' '));
	if (params.empty())
		return this->reply(user, "461 USER :Not enough parameters");
	params.erase(params.begin());
	user.setRealname(params);
	if (!this->_password.empty() && this->_password != user.getPassword())
		return this->reply(user, "464 :Password incorrect");
	user.setIsRegistered(true);
	return
		this->reply(user, "001 " + user.getNickname() + " :Welcome to the Mine " + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname()) &&
		this->reply(user, "002 " + user.getNickname() + " :Your host is " + this->_name + ", running version 1.0") &&
		this->reply(user, "003 " + user.getNickname() + " :This server was created " + this->_creationTime) &&
		this->reply(user, "004 " + user.getNickname() + " :" + this->_name + " " + this->_version + " o");

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
	Server::logMsg(INTERNAL, "\tSocket : " + user.getSocket());
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
		// retRecv = recv(it->second.getSocket(), buff, BUFFER_SIZE, MSG_DONTWAIT);// mettre celle la quand on aura fais fcntl
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
		if (!retRecv) // Check whith the PING
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
	std::string	toSend(": " + msg + "\r\n");

	Server::logMsg(DEBUG, "toSend: [" + toSend + "]");
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
		Server::printUser(this->_users[newUser]);
		this->_users[newUser].setAddr(addr);
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
		for (it = this->_users.begin() ; it != this->_users.end() ; ++it)
		{
			if (!it->second.getIsRegistered())
			{
				Server::logMsg(INTERNAL, "(" + this->toString(it->second.getSocket()) + ") Connection lost");
				this->_users.erase(it);
				break ;
			}
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
	std::make_pair<std::string const, t_fct const>(std::string("MODE"), &Server::cmdMode),
	std::make_pair<std::string const, t_fct const>(std::string("NICK"), &Server::cmdNick),
	std::make_pair<std::string const, t_fct const>(std::string("OPER"), &Server::cmdOper),
	std::make_pair<std::string const, t_fct const>(std::string("PART"), &Server::cmdPart),
	std::make_pair<std::string const, t_fct const>(std::string("PASS"), &Server::cmdPass),
	std::make_pair<std::string const, t_fct const>(std::string("PING"), &Server::cmdPing),
	std::make_pair<std::string const, t_fct const>(std::string("PRIVMSG"), &Server::cmdPrivMsg),
	std::make_pair<std::string const, t_fct const>(std::string("QUIT"), &Server::cmdQuit),
	std::make_pair<std::string const, t_fct const>(std::string("SET"), &Server::cmdSet),
	std::make_pair<std::string const, t_fct const>(std::string("USER"), &Server::cmdUser),
	std::make_pair<std::string const, t_fct const>(std::string(""), NULL),
};

std::pair<enum e_logMsg const, char const *> const	Server::_lookupLogMsgTypes[] = {
	std::make_pair<enum e_logMsg const, char const *>(INTERNAL, WHITE "Internal" RESET),
	std::make_pair<enum e_logMsg const, char const *>(RECEIVED, GREEN "Received" RESET),
	std::make_pair<enum e_logMsg const, char const *>(SENT, MAGENTA "  Sent  " RESET),
	std::make_pair<enum e_logMsg const, char const *>(DEBUG, YELLOW "  Debug " RESET),
};
