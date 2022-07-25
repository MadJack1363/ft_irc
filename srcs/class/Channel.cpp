#include "class/Channel.hpp"

// ************************************************************************** //
//                                Constructors                                //
// ************************************************************************** //

Channel::Channel(std::string const &name) :
	_name(name),
	_modes(0U),
	_users() {}

// ************************************************************************* //
//                                Destructors                                //
// ************************************************************************* //

Channel::~Channel(void)
{
	this->_users.clear();
}

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

std::vector<User *>	&Channel::getUsers(void)
{
	return this->_users;
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

// ************************************************************************* //
//                          Public Member Functions                          //
// ************************************************************************* //


void	Channel::delUser(User &user)
{
	this->_users.erase(std::find(this->_users.begin(), this->_users.end(), &user));
	return ;
}

// TODO Write the function comment
void	Channel::addUser(User &user)
{
	this->_users.push_back(&user);
}

// ************************************************************************** //
//                             Private Attributes                             //
// ************************************************************************** //

/**
 * The available modes are:
 */
std::string const	Channel::_availableModes("");
