#include <cerrno>
#include <cstdlib>

#include <cstdio> // perror

#include <iostream>
#include "color.h"
#include "class/Server.hpp"

inline static int	__getPort(std::string s, uint16_t &port)
{
	for (std::string::const_iterator it = s.begin(); it != s.end(); it++) {
		if (!isdigit(*it))
		{
			std::cerr << "error: port: wrong value" << std::endl;
			return 1;
		}
	}
	port = strtol(s.c_str(), NULL, 10);
	if (errno == ERANGE) {
		std::cerr << "error: port: out of range" << std::endl;
		return 1;
	}
	return 0;
}

int	main(int argc, char **argv)
{
	uint16_t			port;
	Server				server;

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
	if (__getPort(argv[1], port)
		|| !server.init()
		|| !server.start(port))
		return EXIT_FAILURE;

	while (1){
		server.welcomeDwarves();
		server.listenAll();
	}

	close(server.getSocket());
	std::cout << "Project is not working Yet " RED "We are Sorry" RESET << std::endl;
	return EXIT_SUCCESS;
}
