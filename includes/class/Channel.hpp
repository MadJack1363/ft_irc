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

	// REMIND Need to keep it ??
	enum	e_mode
	{
		INVITE_ONLY,
		INSIDE_ONLY,
		PRIVATE,
		SECRET,
	};

	// Attributes
	std::string									_name;
	std::string									_topic;
	std::string									_modes;

	std::map<std::string const, User *const>	_lookupUsers;

	static std::string const					_availableModes;

public:
	// Constructors
	Channel(std::string const &name = "defaultChannelName");

	// Destructors
	virtual ~Channel(void);

	// Member functions
	void														addUser(User &user);
	void														delUser(std::string const &nickname);

	bool														empty(void) const;

	std::map<std::string const, User *const>::iterator			begin(void);
	std::map<std::string const, User *const>::iterator			end(void);
	std::map<std::string const, User *const>::iterator			find(std::string const &nickname);

	std::map<std::string const, User *const>::const_iterator	begin(void) const;
	std::map<std::string const, User *const>::const_iterator	end(void) const;
	std::map<std::string const, User *const>::const_iterator	find(std::string const &nickname) const;

	// Accessors
	std::string const	&getName(void) const;
	std::string const	&getTopic(void) const;
	std::string const	&getModes(void) const;

	static std::string const	&getAvailableModes(void);

	// Mutators
	void	setName(std::string const &name);
	void	setTopic(std::string const &topic);
	void	setModes(std::string const &modes);
};

#endif
