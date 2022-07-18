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

void	Channel::delUser(User &user)
{
	this->_users.erase(std::find(this->_users.begin(), this->_users.end(), &user));
	return ;
}

void	Channel::setName(std::string name)
{
	this->_name = name;
}

std::vector<User *>		&Channel::getUsers(void)
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

/**
 * @brief	Use for add user to the channel
 * 
 * @param user The user add inside the channel
 */
void	Channel::addUser(User &user)
{
	this->_users.push_back(&user);
}

// ************************************************************************** //
//                             Private Attributes                             //
// ************************************************************************** //

std::pair<char const, uint const>	Channel::_lookupModes[] = {
	std::pair<char const, uint const>('s', Channel::SECRET),
	std::pair<char const, uint const>('p', Channel::PRIVATE),
	std::pair<char const, uint const>('i', Channel::INVITE_ONLY),
	std::pair<char const, uint const>('n', Channel::INSIDE_ONLY),
	std::pair<char const, uint const>(0, 0U)
};
