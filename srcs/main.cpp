#include <cstdlib>
#include <cstring> // strstr
#include <cstdio> // perror
#include <poll.h>
#include <iostream>
#include "color.h"
#include "parsing_argv.hpp"
#include "Server.class.hpp"

int	main(int argc, char **argv){
	if (argc != 3){
		std::cerr << "Error use ircserv like " GREEN "./ircserv <port> <password>" RESET << std::endl;
		return (EXIT_FAILURE);
	}

	unsigned short	port;
	if (getPort(argv[1], port))
		return (EXIT_FAILURE);

	std::vector<pollfd>	pollfds;//Attribut prive

	Server	server;
	if (server.start(port) == false)
		return (EXIT_FAILURE);

	pollfds.push_back(pollfd());
	pollfds.back().fd = server.getSocket();
	pollfds.back().events = POLLIN;

	// if (poll(&pollfds[0], pollfds.size(), -1) == -1){
	// 	perror("poll server");
	// 	close(server.getSocket());
	// 	return (EXIT_FAILURE);
	// }

	if (pollfds[0].revents == POLLIN){
		// sockaddr_in addr = {};
		// socklen_t addrlen = sizeof(addr);
		// int newUser = accept(server.getSocket(), reinterpret_cast<sockaddr *>(&addr), &addrlen);

		if (newUser == -1){// chekc if failed
			perror("accept");
			// close(server.getSocket()); // server.stop()
			// return (EXIT_FAILURE);
		}
		pollfds.push_back(pollfd());
		pollfds.back().fd = newUser;
		pollfds.back().events = POLLIN | POLLOUT;
		// need to add a user WITH SOCKET AND ADDR
	}
		// boucle inf

	while (1){
		if (poll(&pollfds[0], pollfds.size(), -1) == -1){// Ecoute les FD donnes
			perror("poll server");//check for error
			// close(server.getSocket());
			// return (EXIT_FAILURE);
		}

		sockaddr_in addr = {};
		socklen_t addrlen = sizeof(addr);
		int newUser = accept(server.getSocket(), reinterpret_cast<sockaddr *>(&addr), &addrlen);
		if (newUser == -1){// chekc if failed
			perror("accept");
		}
		else{
			// have to add to server
			pollfds.push_back(pollfd());
			pollfds.back().fd = newUser;
			pollfds.back().events = POLLIN | POLLOUT;
		}
	}
		// partie accept // check the ping

		// partie poll (setup All socket check recup and receive message)

		// partie recup message (pour la prochaine step du code)

	// }

//	=======================================================================================
	// if (pollfds[0].revents == POLLIN){
	// 	sockaddr_in addr = {};
	// 	socklen_t addrlen = sizeof(addr);
	// 	int newUser = accept(server.getSocket(), reinterpret_cast<sockaddr *>(&addr), &addrlen);
	// 	if (newUser == -1){
	// 		perror("accept");
	// 		close(server.getSocket()); // server.stop()
	// 		return (EXIT_FAILURE);
	// 	}
	// 	pollfds.push_back(pollfd());
	// 	pollfds.back().fd = newUser;
	// 	pollfds.back().events = POLLIN | POLLOUT;
	// }

	if (poll(&pollfds[0], pollfds.size(), -1) == -1){
		perror("poll user");
		close(server.getSocket());
		return (EXIT_FAILURE);
	}

	char		buff[1024];
	size_t		buffsize = sizeof(buff);
	std::string	message;
	std::string	endOfMessage("\r\n");
	bool		EOM = false;
	size_t		retRecv;
	while (1){
		bzero(buff, buffsize);
		while (EOM == false){
			retRecv = recv(pollfds.back().fd, buff, buffsize, 0);
			if (retRecv <= 0){
				perror("recv");
				if (retRecv == 0)
					std::cerr << "retRecv = 0" << std::endl;
				close(pollfds.back().fd);
				close(server.getSocket()); // server.stop()
				return (EXIT_FAILURE);
			}
			message.append(buff);
			if (strstr(buff, endOfMessage.c_str()))
				EOM = true;
		}
		std::cout << "message = [" << message << "]\n";
		if (message.find("QUIT") != std::string::npos)
			break;
		message.clear();
		EOM = false;
		if (send(pollfds.back().fd, ":our.server 001 majacque :Welcome to IRC\r\n", sizeof(":our.server 001 majacque :Welcome to IRC\r\n"), 0) == -1){
			perror("send");
			close(pollfds.back().fd);
			close(server.getSocket());
			return (EXIT_FAILURE);
		}
	}

	close(pollfds.back().fd);
	close(server.getSocket());
	std::cout << "Project is not working Yet " RED "We're Sorry" RESET << std::endl;
	return (EXIT_SUCCESS);
}
