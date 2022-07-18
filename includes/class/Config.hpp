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

public:
	// Constructors
	Config(void);

	// Destructors
	virtual ~Config(void);

	// Member functions
	bool	init(char const *fileName);

	// Operators
	std::string	&operator[](std::string const &key);
};

#endif
