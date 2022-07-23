#include <unistd.h>
#include "class/User.hpp"
#include "class/Server.hpp"

// ************************************************************************** //
//                                Constructors                                //
// ************************************************************************** //

User::User(sockaddr_in const &addr, int sockfd) :
	_addr(addr),
	_socket(sockfd),
	_nickname("*"),
	_username(),
	_servname(),
	_realname(),
	_password(),
	_modes(),
	_msg(),
	_isRegistered(),
	_lastActivity(0),
	_channels() {}

User::User(User const &src) :
	_addr(src._addr),
	_socket(src._socket),
	_nickname(src._nickname),
	_username(src._username),
	_servname(src._servname),
	_realname(src._realname),
	_password(src._password),
	_modes(src._modes),
	_msg(src._msg),
	_isRegistered(src._isRegistered),
	_lastActivity(src._lastActivity),
	_channels(src._channels) {}

// ************************************************************************* //
//                                Destructors                                //
// ************************************************************************* //

User::~User(void)
{
	if (this->_socket != -1)
		close(this->_socket);
	this->_socket = -1;
}

// ************************************************************************* //
//                                 Accessors                                 //
// ************************************************************************* //

sockaddr_in const	&User::getAddr(void) const
{
	return this->_addr;
}

std::string const	&User::getAvailableModes(void)
{
	return User::_availableModes;
}

std::map<std::string, Channel *> const	&User::getChannels(void) const
{
	return this->_channels;
}

std::string const	&User::getHostname(void) const
{
	return this->_hostname;
}

bool const	&User::getIsRegistered(void) const
{
	return this->_isRegistered;
}

std::string const	&User::getModes(void) const
{
	return this->_modes;
}

std::string const	&User::getMsg(void) const
{
	return this->_msg;
}

std::string const	&User::getNickname(void) const
{
	return this->_nickname;
}

std::string const	&User::getPassword(void) const
{
	return this->_password;
}

std::string const	&User::getRealname(void) const
{
	return this->_realname;
}

std::string const	&User::getServname(void) const
{
	return this->_servname;
}

int const	&User::getSocket(void) const
{
	return this->_socket;
}

std::string const	&User::getUsername(void) const
{
	return this->_username;
}

void	User::setAddr(sockaddr_in const &addr)
{
	this->_addr = addr;
}

void	User::setChannels(std::map<std::string, Channel *> const &channels)
{
	this->_channels = channels;
}

void	User::setHostname(std::string const &hostname)
{
	this->_hostname = hostname;
}

void	User::setIsRegistered(bool const isRegistered)
{
	this->_isRegistered = isRegistered;
}

void	User::setMsg(std::string const &msg)
{
	this->_msg = msg;
}

void	User::setModes(std::string const &modes)
{
	this->_modes = modes;
}

void	User::setNickname(std::string const &nickname)
{
	this->_nickname = nickname;
}

void	User::setPassword(std::string const &password)
{
	this->_password = password;
}

void	User::setRealname(std::string const &realname)
{
	this->_realname = realname;
}

void	User::setServname(std::string const &servname)
{
	this->_servname = servname;
}

void	User::setSocket(int const sockfd)
{
	this->_socket = sockfd;
}

void	User::setUsername(std::string const &username)
{
	this->_username = username;
}

// ************************************************************************* //
//                          Public Member Functions                          //
// ************************************************************************* //

bool	User::init(int const &socket, sockaddr_in const &addr)
{
	this->_socket = socket;
	this->_addr = addr;
	return true;
}

// ************************************************************************** //
//                             Private Attributes                             //
// ************************************************************************** //

/**
 * The available modes are:
 * 	- a: away
 * 	- i: invisible
 * 	- o: operator
 */
std::string const	User::_availableModes("aio");
