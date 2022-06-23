#include "../../includes/Server.class.hpp"
#include <cstdio>
#include <arpa/inet.h>
#include <cstring> // strstr

#define PING 10

Server::Server( void ):_socket(-1), _password(), _users(), _channels() {
	return;
}

Server::~Server( void ) {
	return;
}

int	Server::getSocket( void ) const {
	return _socket;
}

bool	Server::start( unsigned short port ) {
	_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (_socket == -1){
		perror("socket");
		return false;
	}
	int optval = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval))){
		perror("setsockopt");
		close(_socket); // stop()
		return false;
	}

	sockaddr_in	addr;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // ip hard code for now
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	if (bind(_socket, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1) {
		perror("bind");
		close(_socket); // stop()
		return false;
	}
	if (listen(_socket, SOMAXCONN) == -1){
		perror("listen");
		close(_socket); // stop()
		return false;
	}
	_pollfds.push_back(pollfd());
	_pollfds.back().fd = _socket;
	_pollfds.back().events = POLLIN;
	return true;
}

bool	Server::welcomeDwarves(void){
	sockaddr_in addr = {};
	socklen_t addrlen = sizeof(addr);
	
	int newUser = accept(_socket, reinterpret_cast<sockaddr *>(&addr), &addrlen);
	if (newUser == -1){
		// perror("accept");
	}
	else{
		std::cout << "New people connected !" <<std::endl;
		_pollfds.push_back(pollfd());
		_pollfds.back().fd = newUser;
		_pollfds.back().events = POLLIN | POLLOUT;
		std::pair<int, User>	new_pair(newUser, User(newUser, addr));
		_users.insert(new_pair);
	}
	if (poll(&_pollfds[0], _pollfds.size(), (PING * 1000) / 10) == -1){// Ecoute les FD donnes
		perror("poll server");//check for error
		return (false);
	}
	return (true);
}

// typedef typename std::map<int, User>::iterator ite_map;

bool	Server::listenAll(void){
	char		buff[1024];
	size_t		buffsize = sizeof(buff);
	std::string	message;
	std::string	endOfMessage("\r\n");
	bool		EOM = false;
	size_t		retRecv;

	if (_users.empty())
		return (true);
	for (std::map<int, User>::iterator ite = _users.begin(); ite != _users.end(); ite++){
		while (EOM == false){
			retRecv = recv(_pollfds.back().fd, buff, buffsize, MSG_DONTWAIT);
			if (retRecv <= 0){
				perror("recv");
				if (retRecv == 0)
					std::cerr << "retRecv = 0" << std::endl;
				return (false);
			}
			message.append(buff);
			if (strstr(buff, endOfMessage.c_str()))
				EOM = true;
		}
		std::cout << "message = " << message;
		if (message.find("QUIT") != std::string::npos)
			break;
		message.clear();
		EOM = false;
		if (send(_pollfds.back().fd, ":our.server 001 majacque :Welcome to IRC\r\n", sizeof(":our.server 001 majacque :Welcome to IRC\r\n"), 0) == -1){
			perror("send");
			return (false);
		}
	}
	return (true);
}

/* void	Server::stop( void ) {
	for (std::map<int, User>::iterator it = _users.begin(); it != _users.end(); it++) {
		it->second.disconnect();
	}
	_users.clear();
	_channels.clear();
	if (_socket != -1) {
		close(_socket);
		_socket = -1;
	}
} */
