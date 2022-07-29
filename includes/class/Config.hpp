#ifndef CONFIG_CLASS_HPP
# define CONFIG_CLASS_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <string>

class Config
{
private:
	std::map<std::string const, std::string>	_lookupValues;

	static std::pair<std::string const, std::string const>	_arrayValues[];
	/*
		server_name,
		server_version,
		modt,
		host,
		max_user,
		ping,
		timeout,
		backlog,
		oper_ + name
	 */

public:
	// Constructors
	Config(void);

	// Destructors
	virtual ~Config(void);

	// Member functions
	bool	init(char const *fileName);

	std::map<std::string const, std::string>::iterator			end(void);
	std::map<std::string const, std::string>::iterator			find(std::string const &key);

	std::map<std::string const, std::string>::const_iterator	end(void) const;
	std::map<std::string const, std::string>::const_iterator	find(std::string const &key) const;

	// Operators
	std::string	&operator[](std::string const &key);

};

#endif
