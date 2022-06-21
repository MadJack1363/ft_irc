/*
         .-"""""""-.
       .'       __  \_
      /        /  \/  \
     |         \_0/\_0/______
     |:.          .'       oo`\
     |:.         /             \
     |' ;        |             |
     |:..   .     \_______     |
     |::.|'     ,  \,_____\   /
     |:::.; ' | .  '|      )_/
     |::; | | ; ; | |             Author:   jodufour
    /::::.|-| |_|-|, \            Contact:  jodufour@student.42.fr
   /'-=-'`  '-'   '--'\           Creation: 2022-06-18 18:49:19
  /                    \
*/

#include "class/User.hpp"

// ************************************************************************** //
//                                Constructors                                //
// ************************************************************************** //

User::User(void) {}

// ************************************************************************* //
//                                Destructors                                //
// ************************************************************************* //

User::~User(void) {}

// ************************************************************************* //
//                                 Accessors                                 //
// ************************************************************************* //

int	User::getSockfd(void) const
{
	return this->_socket;
}

sockaddr_in	User::getAddr(void) const
{
	return this->_addr;
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
