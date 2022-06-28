#include <unistd.h>
#include "class/User.hpp"

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
	_isRegistered(false),
	_isRegisterable(false) {}

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

sockaddr_in	User::getAddr(void) const
{
	return this->_addr;
}

int	User::getSocket(void) const
{
	return this->_socket;
}

std::string	User::getNickname(void) const
{
	return this->_nickname;
}

std::string	User::getUsername(void) const
{
	return this->_username;
}

std::string	User::getHostname(void) const
{
	return this->_hostname;
}

std::string	User::getRealname(void) const
{
	return this->_realname;
}

std::string	User::getPassword(void) const
{
	return this->_password;
}

std::map<std::string, Channel *>	User::getChannels(void) const
{
	return this->_channels;
}

bool	User::getIsOperator(void) const
{
	return this->_isOperator;
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

std::ostream	&operator<<(std::ostream &ostream, User const &user)
{
	ostream << "User : " << std::endl;
	ostream << "\tSocket : " << user.getSocket() << std::endl;
	ostream << "\tNickname : " << user.getNickname() << std::endl;
	ostream << "\tHostname : " << user.getHostname() << std::endl;
	ostream << "\tRealname : " << user.getRealname() << std::endl;
	ostream << "\tPassword : " << user.getPassword() << std::endl; 
	return ostream;
}