#include "../../includes/Server.class.hpp"
#include <cstdio>
#include <arpa/inet.h>

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
	return true;
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
