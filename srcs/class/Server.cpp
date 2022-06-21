/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 14:17:01 by jodufour          #+#    #+#             */
/*   Updated: 2022/06/16 17:46:57 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
bool	Server::start(void)
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
