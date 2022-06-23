#include "User.class.hpp"

User::User(void) : _socket(0), _nickname("*"){

}

User::User(int socket, sockaddr_in in) : _socket(socket), _nickname("*"){
	_addr = in;
}