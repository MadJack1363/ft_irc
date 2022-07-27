#include "class/Channel.hpp"

// ************************************************************************** //
//                             Private Attributes                             //
// ************************************************************************** //

/**
 * The available modes are:
 * 	- i: invite-only
 * 	- n: no outside messages
 */
std::string const	Channel::_availableModes("in");

// ************************************************************************** //
//                                Constructors                                //
// ************************************************************************** //

Channel::Channel(std::string const &name) :
	_name(name),
	_modes(),
	_lookupUsers() {}

// ************************************************************************* //
//                                Destructors                                //
// ************************************************************************* //

Channel::~Channel(void)
{
	this->_lookupUsers.clear();
}

// ************************************************************************* //
//                          Public Member Functions                          //
// ************************************************************************* //

void	Channel::addUser(User &user)
{
	this->_lookupUsers.insert(std::pair<std::string const, User *const>(user.getNickname(), &user));
}

void	Channel::delUser(std::string const &nickname)
{
	this->_lookupUsers.erase(nickname);
}

// std::vector<User *>	const	&Channel::getUsers(void) const
// ************************************************************************* //
//                                 Accessors                                 //
// ************************************************************************* //

std::string const	&Channel::getAvailableModes(void)
{
	return Channel::_availableModes;
}

std::string const	&Channel::getName(void) const
{
	return this->_name;
}

std::string const	&Channel::getTopic(void) const
{
	return this->_topic;
}

std::map<std::string const, User *const> const	&Channel::getLookupUsers(void) const
{
	return this->_lookupUsers;
}

/* time_t const				&Channel::getLastActivity(void) const
{
	return this->_lastActivity;
} */

// ************************************************************************** //
//                                  Mutators                                  //
// ************************************************************************** //

void	Channel::setName(std::string const &name)
{
	this->_name = name;
}

void	Channel::setTopic(std::string const &topic)
{
	this->_topic = topic;
}

void	Channel::setModes(std::string const &modes)
{
	this->_modes = modes;
}

void	Channel::setLookupUsers(std::map<std::string const, User *const> const &lookupUsers)
{
	this->_lookupUsers = lookupUsers;
}
