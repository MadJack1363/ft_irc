#include <cstdlib>
#include <iostream>
#include "color.h"
#include "parsing_argv.hpp"

#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstdio>
int	test_something_socket(void){
	int fd_socket = socket(AF_LOCAL, SOCK_STREAM, 0);

	if (fd_socket == -1){
		perror("");
		return (false);
	}
	std::cerr << "ret socket " << fd_socket << std::endl;
	while (1);
	return (true);
}

#include <signal.h>
#include <poll.h>

// First parameter of fct poll 
// struct pollfd
// {
// 		int		fd;		/* file descriptor */
// 		short	events;	/* requested events */
// 		short	revents;	/* returned events */
// };

int	test_poll(void){
	struct pollfd test;

	nfds_t	nfds = 4;

	int	ret = poll(&test, nfds, 10);// read man poll

	if (ret == -1){
		perror("");
		return (false);
	}
	std::cerr << "ret poll " << ret << std::endl;
	while (1);
	return (true);
}

int	main(int argc, char **argv){
	if (argc != 3){
		std::cerr << "Error use ircserv like " GREEN "./ircserv <port> <password>" RESET << std::endl;
		return (EXIT_FAILURE);
	}
	unsigned int	port;
	std::string		password(argv[2]);

	if (getPort(argv[1], port))
		return (EXIT_FAILURE);
	std::cout << "port = " << port << '\n';
	std::cout << "password = " << password << '\n';

	if (!test_something_socket())
		return (EXIT_FAILURE);

	if (!test_poll())
		return (EXIT_FAILURE);

	std::cout << "Project is not working Yet " RED "We're Sorry" RESET << std::endl;
	return (EXIT_SUCCESS);
}
