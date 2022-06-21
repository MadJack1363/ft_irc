#include "../../includes/Config.class.hpp"
#include <cctype>

Config::Config( void ) {
	for (size_t i = 0; !_initTable[i].first.empty(); i++)
		_val.insert(_initTable[i]);
	return;
}

Config::~Config( void ) {
	_val.clear();
	return;
}

static inline bool	__delSpace( std::string & str ) {
	std::string	tmp(str);
	size_t	i;
	size_t	begin;

	for (i = 0; str[i]; i++){
		if (!isspace(str[i])){
			begin = i;
			break;
		}
	}
	i++;
	for (; str[i]; i++){
		if (isspace(str[i]))
			break;
	}
	str = tmp.substr(begin, i);
	return true;
}

bool	Config::getConfig( char const * fileName ) {
	std::ifstream	infile;
	std::string		line;
	size_t			posEqual;
	std::string		name;
	std::string		value;

	infile.open(fileName);
	if (infile.is_open() == false){
		std::cerr << "Config: getConfig: open failed" << std::endl;
		return false;
	}
	while (infile.good()){
		std::getline(infile, line);
		if (line[0] == '#')
			continue;
		else if ((posEqual = line.find('=')) != std::string::npos){
			name = line.substr(0, posEqual);
			value = line.substr(posEqual + 1, line.size());
			__delSpace(name);
			__delSpace(value);
			_val[name] = value;
			name.clear();
			value.clear();
		}
	}
	if (infile.eof() == false){
		std::cerr << "Config: getConfig: an error occured" << std::endl;
		return false;
	}
	for (std::map<std::string, std::string>::iterator it = _val.begin(); it != _val.end(); it++)
		std::cout << it->first << " = " << it->second << '\n';
	return true;
}

std::pair<std::string, std::string>	Config::_initTable[] = {
	std::make_pair<std::string, std::string>("server_name", "IrcServ"),
	std::make_pair<std::string, std::string>("motd", "config/motd.txt"),
	std::make_pair<std::string, std::string>("host", "127.0.0.1"),
	std::make_pair<std::string, std::string>("max_user", "1024"),
	std::make_pair<std::string, std::string>("ping", "10"),
	std::make_pair<std::string, std::string>("timeout", "30"),
	std::make_pair<std::string, std::string>("backlog", "1024"),
	std::make_pair<std::string, std::string>("oper_name", "admin"),
	std::make_pair<std::string, std::string>("oper_password", "admin"),
	std::make_pair<std::string, std::string>("", "")
};
