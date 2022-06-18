//        .-"""""""-.
//      .'       __  \_
//     /        /  \/  \
//    |         \_0/\_0/______
//    |:.          .'       oo`\
//    |:.         /             \
//    |' ;        |             |
//    |:..   .     \_______     |
//    |::.|'     ,  \,_____\   /
//    |:::.; ' | .  '|      )_/
//    |::; | | ; ; | |             Author:   jodufour
//   /::::.|-| |_|-|, \            Contact:  jodufour@student.42.fr
//  /'-=-'`  '-'   '--'\           Creation: 2022-06-18 10:36:03
// /                    \

#include "Server.class.hpp"

// ************************************************************************** //
//                                Constructors                                //
// ************************************************************************** //

Server::Server(void) {}

// ************************************************************************* //
//                                Destructors                                //
// ************************************************************************* //

Server::~Server(void) {}

// ************************************************************************* //
//                          Public Member Functions                          //
// ************************************************************************* //

/**
 * @brief	Terminates the IRC server.
 * 			This command is reserved for IRC operators.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdDie(void)
{
	return (true);
}

/**
 * @brief	Disconnect the given user from the server.
 * 
 * @param	user The user to disconnect.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdDisconnect(User &user)
{
	return (true);
}

/**
 * @brief	Make the given user joining the channel
 * 			corresponding to the given channel name.
 * 
 * @param	user The user to join the channel.
 * @param	chanName The name of the channel to join.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdJoin(User &user, std::string const &chanName)
{
	return (true);
}

/**
 * @brief	Make the given user being kicked out from the channel
 * 			corresponding to the given channel name.
 * 
 * @param	user The user to kick.
 * @param	chanName The name of the channel to kick the user from.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdKick(User &user, std::string const &chanName)
{
	return (true);
}

/**
 * @brief	Make the given user being removed from the network.
 * 
 * @param	user The user to remove.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdKill(User &user)
{
	return (true);
}

/**
 * @brief	Send a message to either a channel or a user.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdMsg(
	User &user,
	std::string const &target,
	std::string const &msg)
{
	return (true);
}

/**
 * @brief	Make the given user being promoted to operator status.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdOper(
	User &user,
	std::string const &nickname,
	std::string const &password)
{
	return (true);
}

/**
 * @brief	Make the given user leaving the channel
 * 			corresponding to the given channel name.
 * 
 * @param	user The user leaving the channel.
 * @param	chanName The name of the channel to leave.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdPart(User &user, std::string const &chanName)
{
	return (true);
}

/**
 * @brief	Update data of the given user.
 * 
 * @param	user The user to update.
 * @param	key The key/id of the data to update.
 * @param	value The new value of the data.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::cmdSet(
	User &user,
	std::string const &key,
	std::string const &value)
{
	return (true);
}

/**
 * @brief	
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::init(void)
{
	return true;
}

/**
 * @brief	
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::start(uint16_t port)
{
	return true;
}

/**
 * @brief 
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::stop(void)
{
	return true;
}

/**
 * @brief 
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::run(void)
{
	return true;
}

/**
 * @brief 
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::update(void)
{
	return true;
}
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
