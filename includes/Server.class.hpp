#ifndef SERVER_CLASS_HPP
# define SERVER_CLASS_HPP

#include <iostream>
#include <map>
#include <string>
#include <sys/types.h> // socket, bind, listen, recv, send
#include <sys/socket.h>//   "      "      "      "     "
#include <netinet/in.h>// sockaddr_in
#include <unistd.h>// fcntl
#include <fcntl.h> //   "
#include "User.class.hpp"
#include "Channel.class.hpp"

class Server {
private:

	int								_socket;
	std::string						_password;
	
	std::map<int, User>				_users; // int is for id/socket
	std::map<std::string, Channel>	_channels; // string is for the name's channel

public:

	Server( void );
	virtual ~Server( void );

	int		getSocket( void ) const; // XXX temporary

	bool	cmdDie(void);
	bool	cmdDisconnect(User &user);
	bool	cmdJoin(User &user, std::string const &chanName);
	bool	cmdKick(User &user, std::string const &chanName);
	bool	cmdKill(User &user);
	bool	cmdMsg(User &user, std::string const &target, std::string const &msg);
	bool	cmdOper(User &user, std::string const &nickname, std::string const &password);
	bool	cmdPart(User &user, std::string const &chanName);
	bool	cmdSet(User &user, std::string const &key, std::string const &value);

	bool	init(); // get the port, the password & the config (from config files)
	bool	start( uint16_t port ); // socket() + bind() + listen() + fcntl() <-- setup non-blocking fd
	bool	stop( void ); // disconnect all users + _users.clear() + close(_socket) + _socket = INVALID_SOCKET (-1)
	bool	run(); // principal loop (that call update, ect)
	bool	update(); // loop (accept() new users + setup new users) + select()/poll() + loop recv() new messages (1 per user)

	/**
	 * list of command:
	 * 	-set (to change nickname and username)
	 * 	-join
	 * 	-part
	 * 	-msg
	 * 	-quit/disconnect
	 * 	-oper
	 * 	-kick
	 * 	-kill
	 * 	-die
	 * 
	 * optionals:
	 * 	-ping
	 * 	-nick
	 * 	-names
	 * 	-time
	 * 	-ban
	 * 	-kickban
	 */
};

#endif
