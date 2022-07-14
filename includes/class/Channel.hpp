#ifndef CHANNEL_CLASS_HPP
# define CHANNEL_CLASS_HPP

#include <iostream>
#include <algorithm>
#include <vector>
#include "class/User.hpp"

class User;

class Channel {
private:

	std::string			_name;
	std::vector<User *>	_users;

public:

	Channel(std::string name = "Empty");
	virtual ~Channel( void );

	void	addUser(User &user);
	void	delUser(User &user);
	void	setName(std::string name);

	// std::vector<User *>	const	&getUsers(void) const;
	std::vector<User *>			&getUsers(void);
	std::string	const			&getName(void) const;
};

#endif
