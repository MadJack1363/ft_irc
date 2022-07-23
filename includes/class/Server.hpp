#ifndef SERVER_CLASS_HPP
# define SERVER_CLASS_HPP

# include <ctime> // time_t
# include <fcntl.h> //  "
# include <iostream>
# include <list>
# include <map>
# include <netinet/in.h>// sockaddr_in
# include <poll.h>
# include <string>
# include <sys/types.h> // socket, bind, listen, recv, send
# include <sys/socket.h> //   "      "      "      "     "
# include <unistd.h> // fcntl
# include "color.h"
# include "class/User.hpp"
# include "class/Channel.hpp"
# include "class/Config.hpp"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif


class Server
{
private:
	typedef bool	(Server::*t_fct)(User &user, std::string &params);

	enum	e_state
	{
		STOPPED,
		RUNNING,
	};

	enum	e_logMsg
	{
		ERROR,
		INTERNAL,
		RECEIVED,
		SENT,
	};

	enum	e_rplNo
	{
		RPL_WELCOME = 001,
		RPL_YOURHOST = 002,
		RPL_CREATED = 003,
		RPL_MYINFO = 004,

		RPL_UMODEIS = 221,
		RPL_WHOISREGNICK = 307,
		RPL_WHOSIUSER = 311,
		RPL_WHOISOPERATOR = 313,
		RPL_ENDOFWHOIS = 318,
		RPL_WHOISMODES = 379,
		RPL_YOUREOPER = 381,

		ERR_NOSUCHNICK = 401,
		ERR_NOSUCHCHANNEL = 403,
		ERR_NONICKNAMEGIVEN = 431,
		ERR_NICKNAMEINUSE = 433,
		ERR_NEEDMOREPARAMS = 461,
		ERR_ALREADYREGISTRED = 462,
		ERR_PASSWDMISMATCH = 464,
		ERR_UNKNOWNMODE = 472,
		ERR_NOPRIVILEGES = 481,
		ERR_UMODEUNKNOWNFLAG = 501,
		ERR_USERSDONTMATCH = 502,
	};

	// Attributes
	int											_state;
	int											_socket;

	Config										_config;

	std::string									_creationTime;

	std::vector<pollfd>							_pollfds;

	std::list<User>								_users;

	std::map<std::string const, t_fct const>	_lookupCmds;
	std::map<std::string const, User *const>	_lookupUsers;
	std::map<std::string const, Channel>		_lookupChannels;
	std::map<uint const, std::string const>		_lookupLogMsgTypes;
	
	std::list<std::string>						_banList;

	static std::pair<std::string const, t_fct const> const	_arrayCmds[];
	static std::pair<uint const, char const *const> const	_arrayLogMsgTypes[];

	// Member functions
	static std::string	toString(int const nb);

	void	logMsg(uint const type, std::string const &msg);
	void	addToBanList(User const &user);

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
	bool	USER(User &user, std::string &params);
	bool	WHOIS(User &user, std::string &params);
	bool	judge(User &user, std::string &msg);
	bool	recvAll(void);
	bool	replyPush(User &user, std::string const &line);
	bool	replySend(User &user);
	bool	welcomeDwarves(void);

public:
	// Constructors
	Server(void);

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
