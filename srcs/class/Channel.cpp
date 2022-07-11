#include "class/Channel.hpp"

// ************************************************************************** //
//                                Constructors                                //
// ************************************************************************** //

Channel::Channel(std::string name) :
	_name(name),
	_users(),
	_modes(0U) {}

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

std::vector<User *> const	&Channel::getUsers(void) const
{
	return this->_users;
}

std::string const			&Channel::getName(void) const
{
	return this->_name;
}

// ************************************************************************* //
//                          Public Member Functions                          //
// ************************************************************************* //

/**
 * @brief	Get the different available modes for a channel.
 * 
 * @return	The available channel mode identifiers as a string.
 */
std::string	Channel::availableModes(void)
{
	std::string	output;
	uint		idx;

	for (idx = 0U ; Channel::_lookupModes[idx].first ; ++idx)
		output.push_back(Channel::_lookupModes[idx].first);
	return output;
}

// TODO Write the function comment
void	Channel::addUser(User &user)
{
	this->_users.push_back(&user);
}

// ************************************************************************** //
//                             Private Attributes                             //
// ************************************************************************** //

std::pair<char const, uint const>	Channel::_lookupModes[] = {
	std::make_pair('s', Channel::SECRET),
	std::make_pair('p', Channel::PRIVATE),
	std::make_pair('i', Channel::INVITE_ONLY),
	std::make_pair('n', Channel::INSIDE_ONLY),
	std::make_pair(0, 0U)
};
