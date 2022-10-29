#include <cerrno>
#include <cstdlib>
#include <csignal>
#include <iostream>
#include "color.h"
#include "class/Config.hpp"
#include "class/Server.hpp"

bool	g_interrupted = false;

void	sigintHandler(int const sig __attribute__((unused)))
{
	g_interrupted = true;
	std::cout << "\b\b";
}

inline static bool	__getPort(std::string const str, uint16_t &port)
{
	std::string::const_iterator it;

	for (it = str.begin(); it != str.end(); ++it)
	{
		if (!isdigit(*it))
		{
			std::cerr << "error: port: wrong value\n";
			return false;
		}
	}
	port = static_cast<uint16_t>(strtol(str.c_str(), NULL, 10));
	if (errno == ERANGE)
	{
		std::cerr << "error: port: out of range\n";
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
		<< RED_FG "Error: Wrong usage\n"
		<< YELLOW_FG "./ircserv <port> <password>\n"
		<< RESET;
		return EXIT_FAILURE;
	}
	signal(SIGINT, sigintHandler);
	if (!__getPort(argv[1], port) ||
		!server.init(argv[2]) ||
		!server.start(port) ||
		!server.run())
		return EXIT_FAILURE;
	server.stop();
	std::cout << "Project is not working Yet " RED_FG "We are Sorry" RESET << '\n';
	return EXIT_SUCCESS;
}
