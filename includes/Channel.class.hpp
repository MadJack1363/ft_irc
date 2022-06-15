#ifndef CHANNEL_CLASS_HPP
# define CHANNEL_CLASS_HPP

#include <iostream>
#include <vector>
#include "User.class.hpp"

class Channel {
private:

	std::vector<User &>	_users;

public:

	Channel( void );
	Channel( Channel const & rhs );
	virtual ~Channel( void );

	Channel&	operator=( Channel const & rhs );

};

#endif
