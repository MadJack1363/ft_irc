#ifndef USER_CLASS_HPP
# define USER_CLASS_HPP

#include <iostream>
#include <map>
#include <netinet/in.h>// sockaddr_in
#include <string>
#include <sys/types.h> // socket, bind, listen, recv, send
#include <sys/socket.h> //   "      "      "      "     "
#include "class/Channel.hpp"

class Channel;

class User
{
private:
	// Attributes
	sockaddr_in									_addr;

	int											_socket;

	std::string									_nickname; // Max length is 9 chars
	std::string									_username;
	std::string									_hostname;
	std::string									_realname;
	std::string									_password;

	bool										_isRegistered;

	uint8_t										_modes;

	time_t										_lastActivity;

	std::map<std::string, Channel *>			_channels; // ??

	static std::pair<char const, uint const>	_lookupModes[];

public:
	enum	e_mode
	{
		AWAY,
		OPERATOR,
		INVISIBLE,
	};

	// Constructors
	User(sockaddr_in const &addr = sockaddr_in(), int sockfd = -1);
	User(User const &src);

	// Destructors
	virtual ~User(void);

	// Accessors
	sockaddr_in const						&getAddr(void) const;

	int const								&getSocket(void) const;

	std::string const						&getNickname(void) const;
	std::string const						&getUsername(void) const;
	std::string const						&getHostname(void) const;
	std::string const						&getRealname(void) const;
	std::string const						&getPassword(void) const;

	bool const								&getIsRegistered(void) const;

	uint8_t const							&getModes(void) const;

	time_t const							&getLastActivity(void) const;

	std::map<std::string, Channel *> const	&getChannels(void) const;

	void									setSocket(int const sockfd);
	void									setAddr(sockaddr_in const &addr);
	void									setNickname(std::string const &nickname);
	void									setUsername(std::string const &username);
	void									setHostname(std::string const &hostname);
	void									setRealname(std::string const &realname);
	void									setPassword(std::string const &password);
	void									setIsRegistered(bool const isRegistered);
	void									setModes(uint8_t const modes);
	void									setChannels(std::map<std::string, Channel *> const &channels);

	// Member functions
	static std::string	availableModes(void);

	void		addMode(char const c);
	void		delMode(char const c);

	void		updateLastActivity(void);

	bool		init(int const &socket, sockaddr_in const &addr); // set _socket & _addr + fcntl() <-- setup non-blocking fd
	bool		sendTo(User const & user); // send private message
	bool		sendToAll(Channel const & chan); // send message to every user in the channel (except myself)

	std::string	activeModes(void) const;
};

#endif
