#ifndef USER_CLASS_HPP
# define USER_CLASS_HPP

#include <iostream>
#include <string>
#include <sys/types.h> // socket, bind, listen, recv, send
#include <sys/socket.h>//   "      "      "      "     "
#include <netinet/in.h>// sockaddr_in
#include "Channel.class.hpp"

class Channel;

class User {
private:

	int			_socket;
	sockaddr_in	_addr;

	std::string	_nickname;
	std::string	_username;
	std::string	_password;

	bool		isOperator;

public:

	User( void );
	User(int socket, sockaddr_in in);
	virtual ~User( void ) {};

	bool	init( int const & socket, sockaddr_in const & addr ); // set _socket & _addr + fcntl() <-- setup non-blocking fd

	// ADDUSER(socket, addr);
	void	disconnect( void );

	bool	sendTo( User const & user ); // send private message
	bool	sendToAll( Channel const & chan ); // send message to every user in the channel (except myself)
};

#endif
