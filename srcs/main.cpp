#include <cerrno>
#include <cstdlib>

#include <cstdio> // perror

#include <iostream>
#include "color.h"
#include "class/Server.hpp"

inline static bool	__getPort(std::string const str, uint16_t &port)
{
	std::string::const_iterator it;

	for (it = str.begin(); it != str.end(); ++it)
	{
		if (!isdigit(*it))
		{
			std::cerr << "error: port: wrong value" << std::endl;
			return false;
		}
	}
	port = strtol(str.c_str(), NULL, 10);
	if (errno == ERANGE)
	{
		std::cerr << "error: port: out of range" << std::endl;
		return false;
	}
	return true;
}

int	main(int const argc, char const *const *const argv)
{
	Server		server;
	uint16_t	port;

	if (argc != 3)
	{
		std::cerr
		<< RED
		<< "Error: Wrong usage"
		<< '\n'
		<< YELLOW
		<< "" GREEN "./ircserv <port> <password>" RESET << std::endl;
		return EXIT_FAILURE;
	}
	if (!__getPort(argv[1], port) ||
		!server.init(argv[2]) ||
		!server.start(port) ||
		!server.run())
		return EXIT_FAILURE;
	std::cout << "Project is not working Yet " RED "We are Sorry" RESET << std::endl;
	return EXIT_SUCCESS;
}
