#include <cstdlib>
#include <iostream>
#include "color.h"

int	main(int argc, char **argv){
	int i;

	(void)argv;
	if (argc != 3){
		std::cerr << "Error use ircserv like " GREEN "./ircserv <port> <password>" RESET << std::endl;
		return (EXIT_FAILURE);
	}
	std::cerr << "Project is not working Yet " RED "We're Sorry" RESET << std::endl;
	return (EXIT_SUCCESS);
}