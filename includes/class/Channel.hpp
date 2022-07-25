#ifndef CHANNEL_CLASS_HPP
# define CHANNEL_CLASS_HPP

#include <iostream>
#include <algorithm>
#include <vector>
#include "class/User.hpp"


class User;

class Channel
{
private:
	enum	e_mode
	{
		INVITE_ONLY,
		INSIDE_ONLY,
		PRIVATE,
		SECRET,
	};

	// Attributes
	std::string			_name;
	std::string			_modes;

	std::vector<User *>	_users;

	static std::string const	_availableModes;

public:
	// Constructors
	Channel(std::string const &name = "Empty");

	// Destructors
	virtual ~Channel(void);

	// Accessors
	std::vector<User *>			&getUsers(void);
	std::string	const			&getName(void) const;

	static std::string const	&getAvailableModes(void);

	// Mutators
	void	setName(std::string const &name);
	void	setUsers(std::vector<User *> const &users);

	// Member functions
	void	addUser(User &user);
	void	delUser(User &user);
};

#endif
