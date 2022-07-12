#include <cstdlib>
#include <iostream>
#include "color.h"
#include "parsing_argv.hpp"
#include "../includes/Config.class.hpp"

int	main(int argc, char **argv){
	/* if (argc != 3){
		std::cerr << "Error use ircserv like " GREEN "./ircserv <port> <password>" RESET << std::endl;
		return (EXIT_FAILURE);
	}
	unsigned int	port;
	std::string		password(argv[2]);

	if (getPort(argv[1], port))
		return (EXIT_FAILURE);
	std::cout << "port = " << port << '\n';
	std::cout << "password = " << password << '\n';

	std::cout << "Project is not working Yet " RED "We're Sorry" RESET << std::endl; */
	
	Config	conf;

	conf.getConfig("config/default.conf");
	for (std::map<std::string, std::string>::iterator it = conf._val.begin(); it != conf._val.end() ; it++)
	{
		std::cout << it->first << " = " << it->second << '\n';
	}
	
	return (EXIT_SUCCESS);
}
