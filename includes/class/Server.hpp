#ifndef SERVER_CLASS_HPP
# define SERVER_CLASS_HPP

# include <iostream>
# include <map>
# include <string>
# include <sys/types.h> // socket, bind, listen, recv, send
# include <sys/socket.h>//   "      "      "      "     "
# include <netinet/in.h>// sockaddr_in
# include <unistd.h>// fcntl
# include <fcntl.h> //   "
# include <poll.h>
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
};

enum	e_rplNo
{
	RPL_WELCOME = 001,
	RPL_YOUREOPER = 381,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_NICKNAMEINUSE = 433,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTRED = 462,
	ERR_PASSWDMISMATCH = 464,
};

typedef bool	(Server::*t_fct)(User &user, std::string &params);

class Server
{
private:
	// Attributes
	int												_state;
	int												_socket;

	std::string										_name;
	std::string										_password;

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

	bool	cmdDie(User &user, std::string &params);
	bool	cmdJoin(User &user, std::string &params);
	bool	cmdKick(User &user, std::string &params);
	bool	cmdKill(User &user, std::string &params);
	bool	cmdMsg(User &user, std::string &params);
	bool	cmdNick(User &user, std::string &params);
	bool	cmdOper(User &user, std::string &params);
	bool	cmdPart(User &user, std::string &params);
	bool	cmdPass(User &user, std::string &params);
	bool	cmdPing(User &user, std::string &params);
	bool	cmdQuit(User &user, std::string &params);
	bool	cmdSet(User &user, std::string &params);
	bool	cmdUser(User &user, std::string &params);
	bool	judge(User &user, std::string &msg);
	bool	recvAll(void);
	bool	reply(User const &user, std::string const &msg) const;
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
