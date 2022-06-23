#include <cstdlib>

#include <cstdio> // perror

#include <iostream>
#include "color.h"
#include "parsing_argv.hpp"
#include "Server.class.hpp"

int	main(int argc, char **argv){
	if (argc != 3){
		std::cerr << "Error use ircserv like " GREEN "./ircserv <port> <password>" RESET << std::endl;
		return (EXIT_FAILURE);
	}
	//Config port
	unsigned short	port;
	if (getPort(argv[1], port))
		return (EXIT_FAILURE);

	//Start the server
	Server	server;
	if (server.start(port) == false)
		return (EXIT_FAILURE);

	size_t i = 0;

	while (1){
		std::cout << "===================================\nturn = " << i << std::endl;
		server.welcomeDwarves();
		server.listenAll();
		i++;
	}

	// close(pollfds.back().fd);
	close(server.getSocket());
	std::cout << "Project is not working Yet " RED "We're Sorry" RESET << std::endl;
	return (EXIT_SUCCESS);
}
