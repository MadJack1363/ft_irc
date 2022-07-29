#include "class/Server.hpp"

/**
 * @brief	Send de Message of the Day to the user.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */

bool	Server::MOTD(User &user, std::string const &params)
{
	std::ifstream	infile;
	std::string		line;
	std::string		motd;

	if (params.empty() == false && params.compare(this->_config["host"]) != 0)
		return this->replyPush(user, ":" + this->_config["host"] + " 402 " + user.getNickname() + " " + params + " :No such server");
	
	infile.open(this->_config["motd"].c_str());
	if (infile.is_open() == false)
		return this->replyPush(user, ":" + this->_config["host"] + " 422 " + user.getNickname() + " :MOTD File is missing");

	this->replyPush(user, /* ":" + this->_config["host"] + "  */"375 " + user.getNickname() + " :- Hello Digger! -");

	while (infile.good() && std::getline(infile, line))
	{
		motd.append(line + "\n");
	}
	infile.close();
	return this->replyPush(user, /* ":" + this->_config["host"] + "  */"372 " + user.getNickname() + " :" + motd)
		&& this->replyPush(user, "376 " + user.getNickname() + " :End of /MOTD command");
}
