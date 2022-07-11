#ifndef SERVER_CLASS_HPP
# define SERVER_CLASS_HPP

# include <ctime> // time_t
# include <fcntl.h> //   "
# include <iostream>
# include <map>
# include <netinet/in.h>// sockaddr_in
# include <poll.h>
# include <string>
# include <sys/types.h> // socket, bind, listen, recv, send
# include <sys/socket.h>//   "      "      "      "     "
# include <unistd.h>// fcntl
# include "color.h"
# include "class/User.hpp"
# include "class/Channel.hpp"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif

class Server;

enum	e_state
{
	STOPPED,
	RUNNING,
};

enum	e_logMsg
{
	INTERNAL,
	RECEIVED,
	SENT,
	DBG,
};

enum	e_rplNo
{
	RPL_WELCOME = 001,
	RPL_YOURHOST = 002,
	RPL_CREATED = 003,
	RPL_MYINFO = 004,
	RPL_YOUREOPER = 381,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_NICKNAMEINUSE = 433,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTRED = 462,
	ERR_PASSWDMISMATCH = 464,
	ERR_UNKNOWNMODE = 472,
};

typedef bool	(Server::*t_fct)(User &user, std::string &params);

class Server
{
private:
	// Attributes
	int												_state;
	int												_socket;

	std::string										_ip;
	std::string										_msg;
	std::string										_name;
	std::string										_version;
	std::string										_password;
	std::string										_creationTime;
	std::string										_availableUserModes;
	std::string										_availableChannelModes;

	std::vector<pollfd>								_pollfds;

	std::map<int, User>								_users; // int is for id/socket
	std::map<std::string, Channel>					_channels; // string is for the name's channel
	std::map<std::string const, t_fct const>		_cmds;

	static std::pair<std::string const, t_fct const> const		_lookupCmds[];
	static std::pair<enum e_logMsg const, char const *> const	_lookupLogMsgTypes[];

	// Member functions
	static void	logMsg(enum e_logMsg const type, std::string const &msg);
	static void	printUser(User const &user);

	static std::string	toString(int const nb);


	bool	DIE(User &user, std::string &params);
	bool	JOIN(User &user, std::string &params);
	bool	KICK(User &user, std::string &params);
	bool	KILL(User &user, std::string &params);
	bool	MODE(User &user, std::string &params);
	bool	NICK(User &user, std::string &params);
	bool	OPER(User &user, std::string &params);
	bool	PART(User &user, std::string &params);
	bool	PASS(User &user, std::string &params);
	bool	PING(User &user, std::string &params);
	bool	PRIVMSG(User &user, std::string &params);
	bool	QUIT(User &user, std::string &params);
	bool	SET(User &user, std::string &params);
	bool	USER(User &user, std::string &params);
	bool	judge(User &user, std::string &msg);
	bool	recvAll(void);
	bool	replyPush(std::string const &line);
	bool	replySend(User const &user);
	bool	welcomeDwarves(void);

public:
	// Constructors
	Server( void );

	// Destructors
	virtual ~Server(void);

	// Member functions
	void	stop(void);

	bool	init(std::string const password);
	bool	run(void);
	bool	start(uint16_t const port);


	/**
	 * optionals command:
	 * 
	 * 	-ping
	 * 	-nick
	 * 	-names
	 * 	-time
	 * 	-ban
	 * 	-kickban
	 */
};

#endif
