#include <sys/types.h>
#include "class/Config.hpp"

// ************************************************************************** //
//                                Constructors                                //
// ************************************************************************** //

Config::Config(void)
{
	uint	idx;

	for (idx = 0U ; !Config::_arrayValues[idx].first.empty() ; ++idx)
		this->_lookupValues.insert(_arrayValues[idx]);
}

// ************************************************************************* //
//                                Destructors                                //
// ************************************************************************* //

Config::~Config(void)
{
	this->_lookupValues.clear();
}

// ************************************************************************* //
//                          Public Member Functions                          //
// ************************************************************************* //

/**
 * @brief	Load the configuration file to initialize values.
 * 
 * @param	fileName The name of the configuration file.
 * 
 * @return	true if success, false otherwise.
 */
bool	Config::init(char const *fileName)
{
	std::ifstream	infile;
	std::string		line;
	std::string		name;
	std::string		value;
	size_t			posEqual;

	infile.open(fileName);
	if (infile.is_open() == false)
	{
		std::cerr << "Config: getConfig: open() failed\n";
		return false;
	}
	while (infile.good() && std::getline(infile, line))
	{
		if (line[0] == '#')
			continue ;
		else if ((posEqual = line.find('=')) != std::string::npos)
		{
			name = line.substr(0, posEqual);
			value = line.substr(posEqual + 1, line.size());
			name.erase(0, name.find_first_not_of(' '));
			name.erase(name.find_last_not_of(' ') + 1);
			value.erase(0, value.find_first_not_of(' '));
			value.erase(value.find_last_not_of(' ') + 1);
			if (this->_lookupValues.find(name) != this->_lookupValues.end())
				this->_lookupValues[name] = value;
			name.clear();
			value.clear();
		}
	}
	if (infile.eof() == false)
	{
		std::cerr << "Config: getConfig: an error occured\n";
		return false;
	}
	return true;
}

// ************************************************************************* //
//                                 Operators                                 //
// ************************************************************************* //

/**
 * @brief	Access to an element of the Config.
 * 
 * @param	key The key of the element to access.
 * 
 * @return	The value of the element.
 */
std::string	&Config::operator[](std::string const &key)
{
	return this->_lookupValues[key];
}

// ************************************************************************** //
//                             Private Attributes                             //
// ************************************************************************** //

std::pair<std::string const, std::string const>	Config::_arrayValues[] = {
	std::pair<std::string const, std::string const>("server_name", "Khazad-Dum"),
	std::pair<std::string const, std::string const>("server_version", "1.0"),
	std::pair<std::string const, std::string const>("motd", "config/motd.txt"),
	std::pair<std::string const, std::string const>("host", "127.0.0.1"),
	std::pair<std::string const, std::string const>("max_user", "1024"),
	std::pair<std::string const, std::string const>("ping", "10"),
	std::pair<std::string const, std::string const>("timeout", "30"),
	std::pair<std::string const, std::string const>("backlog", "1024"),
	std::pair<std::string const, std::string const>("oper_name", "admin"),
	std::pair<std::string const, std::string const>("oper_password", "admin"),
	std::pair<std::string const, std::string const>("", "")
};
