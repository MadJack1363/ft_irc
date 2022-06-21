#ifndef CHANNEL_CLASS_HPP
# define CHANNEL_CLASS_HPP

#include <iostream>
#include <vector>
#include "class/User.hpp"

class User;

class Channel {
private:

	std::vector<User *>	_users;

public:

	Channel( void );
	virtual ~Channel( void ) {};

};

#endif
