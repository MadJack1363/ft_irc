#ifndef CHANNEL_CLASS_HPP
# define CHANNEL_CLASS_HPP

#include <iostream>
#include <vector>
#include "User.class.hpp"

class User;

class Channel {
private:

	std::vector<User *>	_users;

public:

	Channel( void );
	virtual ~Channel( void ) {};

};

#endif
