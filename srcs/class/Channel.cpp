#include "class/Channel.hpp"

Channel::Channel(void)
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