#ifndef CONFIG_CLASS_HPP
# define CONFIG_CLASS_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <map>


class Config {
public:

	static std::pair<std::string, std::string>	_initTable[];

	std::map<std::string, std::string>			_val;

	Config( void );
	virtual ~Config( void );

	bool	getConfig( char const * fileName );

};

#endif
