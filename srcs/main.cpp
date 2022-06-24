#include <cerrno>
#include <cstdlib>
#include <cstring> // strstr
#include <cstdio> // perror
#include <poll.h>
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
	std::vector<pollfd>	pollfds;
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

	pollfds.push_back(pollfd());
	pollfds.back().fd = server.getSocket();
	pollfds.back().events = POLLIN;

	if (poll(&pollfds[0], pollfds.size(), -1) == -1)
	{
		close(server.getSocket());
		perror("poll server");
		return EXIT_FAILURE;
	}

	if (pollfds[0].revents == POLLIN)
	{
		sockaddr_in addr = {};
		socklen_t addrlen = sizeof(addr);
		int newUser = accept(server.getSocket(), reinterpret_cast<sockaddr *>(&addr), &addrlen);
		if (newUser == -1)
		{
			perror("accept");
			close(server.getSocket()); // server.stop()
			return EXIT_FAILURE;
		}
		pollfds.push_back(pollfd());
		pollfds.back().fd = newUser;
		pollfds.back().events = POLLIN | POLLOUT;
	}

	if (poll(&pollfds[0], pollfds.size(), -1) == -1)
	{
		perror("poll user");
		close(server.getSocket());
		return EXIT_FAILURE;
	}

	char		buff[1024];
	size_t		buffsize = sizeof(buff);
	std::string	msg;
	bool		EOM = false;
	size_t		retRecv;
	User		bob; // Default user for tests, to be removed.

	bob.setSocket(pollfds.back().fd);
	bob.setNickname("Bobby42");
	bob.setUsername("Bob");
	bob.setHostname("BobLaptop");
	bob.setRealname("Bob MARLEY");
	bob.setPassword("ThisIsMyPassword");

	while (server.getState() == RUNNING)
	{
		bzero(buff, buffsize);
		while (EOM == false)
		{
			retRecv = recv(pollfds.back().fd, buff, buffsize, 0);
			if (retRecv <= 0)
			{
				close(pollfds.back().fd);
				close(server.getSocket()); // server.stop()
				perror("recv");
				if (retRecv == 0)
					std::cerr << "retRecv == 0" << std::endl;
				return EXIT_FAILURE;
			}
			msg.append(buff);
			if (strstr(buff, "\r\n"))
				EOM = true;
		}
		server.judge(bob, msg);
		msg.clear();
		EOM = false;

		// Sending back to client a RPL_WELCOME
		msg = "001 " + bob.getNickname() + ":Gimli gimli gimli a dwarf after midnight\r\n";
		if (send(pollfds.back().fd, msg.c_str(), msg.size() + 1, 0) == -1)
		{
			perror("send");
			close(pollfds.back().fd);
			close(server.getSocket());
			return EXIT_FAILURE;
		}
		msg.clear();
	}

	close(pollfds.back().fd);
	close(server.getSocket());
	std::cout << "Project is not working Yet " RED "We are Sorry" RESET << std::endl;
	return EXIT_SUCCESS;
}
