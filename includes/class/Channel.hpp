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

	std::vector<User *>	_users;

	uint8_t				_modes;

	static std::pair<char const, uint const>	_lookupModes[];

public:
	// Constructors
	Channel(std::string name = "Empty");

	void	addUser(User &user);
	void	delUser(User &user);
	void	setName(std::string name);

	// Destructors
	virtual ~Channel(void);

	// Accessors
	std::vector<User *>			&getUsers(void);
	std::string	const			&getName(void) const;
	// Member functions
	static std::string	availableModes(void);
};

#endif
