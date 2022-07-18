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
	_hostname(),
	_realname(),
	_password(),
	_isRegistered(),
	_modes(),
	_channels()
{
	time(&_lastActivity);
}

User::User(User const &src) :
	_addr(src._addr),
	_socket(src._socket),
	_nickname(src._nickname),
	_username(src._username),
	_hostname(src._hostname),
	_realname(src._realname),
	_password(src._password),
	_isRegistered(src._isRegistered),
	_modes(src._modes),
	_channels(src._channels),
	_lastActivity(src._lastActivity) {}

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

std::string const	&User::getMsg(void) const
{
	return this->_msg;
}

bool const	&User::getIsRegistered(void) const
{
	return this->_isRegistered;
}

uint8_t const	&User::getModes(void) const
{
	return this->_modes;
}

time_t const	&User::getLastActivity(void) const
{
	return this->_lastActivity;
}

std::map<std::string, Channel *> const	&User::getChannels(void) const
{
	return this->_channels;
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

void	User::setMsg(std::string const &msg)
{
	this->_msg = msg;
}

void	User::setIsRegistered(bool const isRegistered)
{
	this->_isRegistered = isRegistered;
}

void	User::setModes(uint8_t const modes)
{
	this->_modes = modes;
}

void	User::setChannels(std::map<std::string, Channel *> const &channels)
{
	this->_channels = channels;
}

// ************************************************************************* //
//                          Public Member Functions                          //
// ************************************************************************* //

/**
 * @brief	Get the active modes as a string.
 * 
 * @return	The active modes as a string.
 */
std::string	User::activeModes(void) const
{
	std::string	output;
	uint		idx;

	for (idx = 0U ; User::_lookupModes[idx].first ; ++idx)
		if (this->_modes & (1 << User::_lookupModes[idx].second))
			output.push_back(User::_lookupModes[idx].first);
	return output;
}

/**
 * @brief	Activate a specific mode.
 * 
 * @param	c The identifier of the mode to activate.
 */
void	User::addMode(char const c)
{
	uint	idx;

	for (idx = 0U ; User::_lookupModes[idx].first && c != User::_lookupModes[idx].first ; ++idx);
	if (User::_lookupModes[idx].first)
		this->_modes |= 1 << User::_lookupModes[idx].second;
}

/**
 * @brief	Get the different available modes for an user.
 * 
 * @return	The available user mode identifiers as a string.
 */
std::string	User::availableModes(void)
{
	std::string	output;
	uint		idx;

	for (idx = 0U ; User::_lookupModes[idx].first ; ++idx)
		output.push_back(User::_lookupModes[idx].first);
	return output;
}

/**
 * @brief	Deactivate a specific mode.
 * 
 * @param	c The identifier of the mode to deactivate.
 */
void	User::delMode(char const c)
{
	uint	idx;

	for (idx = 0U ; User::_lookupModes[idx].first && c != User::_lookupModes[idx].first ; ++idx);
	if (User::_lookupModes[idx].first)
		this->_modes &= ~(1 << User::_lookupModes[idx].second);
}

/**
 * @brief Update the value _lastActivity for the ping
 * 
 */
void	User::updateLastActivity(void)
{
	time(&this->_lastActivity);
}

// ************************************************************************** //
//                             Private Attributes                             //
// ************************************************************************** //

std::pair<char const, uint const>	User::_lookupModes[] =
{
	std::pair<char const, uint const>('a', User::AWAY),
	std::pair<char const, uint const>('o', User::OPERATOR),
	std::pair<char const, uint const>('i', User::INVISIBLE),
	std::pair<char const, uint const>(0, 0U)
};
