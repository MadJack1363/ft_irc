#include <unistd.h>
#include "class/User.hpp"
#include "class/Server.hpp"

// ************************************************************************** //
//                                Constructors                                //
// ************************************************************************** //

User::User(void) :
	_socket(-1),
	_addr(),
	_nickname("*"),
	_username(),
	_hostname(),
	_realname(),
	_password(),
	_channels(),
	_isOperator(false),
	_isRegistered(false) {}

// ************************************************************************* //
//                                Destructors                                //
// ************************************************************************* //

User::~User(void)
{
	if (this->_socket >= 0)
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

int const	&User::getSocket(void) const
{
	return this->_socket;
}

std::string const	&User::getNickname(void) const
{
	return this->_nickname;
}

std::string const	&User::getUsername(void) const
{
	return this->_username;
}

std::string const	&User::getHostname(void) const
{
	return this->_hostname;
}

std::string const	&User::getRealname(void) const
{
	return this->_realname;
}

std::string const	&User::getPassword(void) const
{
	return this->_password;
}

std::map<std::string, Channel *> const	&User::getChannels(void) const
{
	return this->_channels;
}

bool const	&User::getIsOperator(void) const
{
	return this->_isOperator;
}

bool const	&User::getIsRegisterable(void) const
{
	return this->_isRegisterable;
}

bool const	&User::getIsRegistered(void) const
{
	return this->_isRegistered;
}

void	User::setSocket(int const sockfd)
{
	this->_socket = sockfd;
}

void	User::setAddr(sockaddr_in const &addr)
{
	this->_addr = addr;
}

void	User::setNickname(std::string const &nickname)
{
	this->_nickname = nickname;
}

void	User::setUsername(std::string const &username)
{
	this->_username = username;
}

void	User::setHostname(std::string const &hostname)
{
	this->_hostname = hostname;
}

void	User::setRealname(std::string const &realname)
{
	this->_realname = realname;
}

void	User::setPassword(std::string const &password)
{
	this->_password = password;
}

void	User::setChannels(std::map<std::string, Channel *> const &channels)
{
	this->_channels = channels;
}

void	User::setIsOperator(bool const isOperator)
{
	this->_isOperator = isOperator;
}

void	User::setIsRegistered(bool const isRegistered)
{
	this->_isRegistered = isRegistered;
}

