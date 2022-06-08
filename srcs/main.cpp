#include <cstdlib>
#include <iostream>
#include "color.h"

int	main(int argc, char **argv){
	(void)argv;
	if (argc != 3){
		std::cerr << "Error use ircserv like " <<  GREEN << "./ircserv <port> <password>" << RESET << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}