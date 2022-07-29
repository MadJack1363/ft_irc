#include "class/Channel.hpp"

// ************************************************************************** //
//                             Private Attributes                             //
// ************************************************************************** //

/**
 * The available modes are:
 * - no any for now
 */
std::string const	Channel::_availableModes("");

// ************************************************************************** //
//                                Constructors                                //
// ************************************************************************** //

Channel::Channel(std::string const &name) :
	_name(name),
	_topic(),
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

/**
 * @brief	Add a new user to the channel,
 * 			or modify the user known as its nickname
 * 			if such an user already exists in the channel.
 * 
 * @param	user The user to add.
 */
void	Channel::addUser(User &user)
{
	this->_lookupUsers.insert(std::pair<std::string const, User *const>(user.getNickname(), &user));
}

/**
 * @brief	Get an iterator to the first user of the channel.
 * 
 * @return	An iterator to the first user of the channel.
 */
std::map<std::string const, User *const>::iterator	Channel::begin(void)
{
	return this->_lookupUsers.begin();
}

/**
 * @brief	Get a const_iterator to the first user of the channel.
 * 
 * @return	A const_iterator to the first user of the channel.
 */
std::map<std::string const, User *const>::const_iterator	Channel::begin(void) const
{
	return this->_lookupUsers.begin();
}

/**
 * @brief	Remove an user from the channel.
 * 
 * @param	nickname The nickname of the user to remove.
 */
void	Channel::delUser(std::string const &nickname)
{
	this->_lookupUsers.erase(nickname);
}

/**
 * @brief	Check if the channel is empty.
 * 
 * @return	Either true if the channel is empty, or false if not.
 */
bool	Channel::empty(void) const
{
	return this->_lookupUsers.empty();
}

/**
 * @brief	Get an iterator to the post-last user of the channel.
 * 
 * @return	An iterator to the post-last user of the channel.
 */
std::map<std::string const, User *const>::iterator	Channel::end(void)
{
	return this->_lookupUsers.end();
}

/**
 * @brief	Get a const_iterator to the post-last user of the channel.
 * 
 * @return	A const_iterator to the post-last user of the channel.
 */
std::map<std::string const, User *const>::const_iterator	Channel::end(void) const
{
	return this->_lookupUsers.end();
}

/**
 * @brief	Get an iterator to an user with a given nickname.
 * 
 * @param	nickname The nickname of the user to find.
 * 
 * @return	Either an iterator to the user with the given nickname,
 * 			or the end() iterator if no user with the given nickname is found.
 */
std::map<std::string const, User *const>::iterator	Channel::find(std::string const &nickname)
{
	return this->_lookupUsers.find(nickname);
}

/**
 * @brief	Get a const_iterator to an user with a given nickname.
 * 
 * @param	nickname The nickname of the user to find.
 * 
 * @return	Either a const_iterator to the user with the given nickname,
 * 			or the end() const_iterator if no user with the given nickname is found.
 */
std::map<std::string const, User *const>::const_iterator	Channel::find(std::string const &nickname) const
{
	return this->_lookupUsers.find(nickname);
}

// ************************************************************************* //
//                                 Accessors                                 //
// ************************************************************************* //

std::string const	&Channel::getAvailableModes(void)
{
	return Channel::_availableModes;
}

std::string const	&Channel::getModes(void) const
{
	return this->_modes;
}

std::string const	&Channel::getName(void) const
{
	return this->_name;
}

std::string const	&Channel::getTopic(void) const
{
	return this->_topic;
}

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
