#include "class/Channel.hpp"

Channel::Channel(std::string name) : _name(name), _users()
{

}

Channel::~Channel(void)
{
	this->_users.clear();
}

void	Channel::addUser(User &user)
{
	this->_users.push_back(&user);
	return ;
}

void	Channel::delUser(User &user)
{
	this->_users.erase(std::find(this->_users.begin(), this->_users.end(), &user));
	return ;
}

std::vector<User *>	const		&Channel::getUsers(void) const
{
	return this->_users;
}

std::string const			&Channel::getName(void) const
{
	return this->_name;
}