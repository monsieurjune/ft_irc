/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:31:35 by tponutha          #+#    #+#             */
/*   Updated: 2025/01/21 18:41:57 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

// C Header
#include <ctime>

// CPP Header
#include <string>
#include <set>
#include <queue>

// Forward Decalration to avoid CIRCULAR include
class Channel;

/**
 * @class Client
 * @brief A Class For Encapsulate IRC Client
 * 
 * This Class is Use for Encapsulate IRC Socket And Its assosiated properties.
 */
class Client
{
	private:
		/**
		 * @brief The socket's fd of Client
		 */
		int	_fd;
		
		/**
		 * @brief Time when client is connected
		 * 
		 * @note It should be time when Client is constructed
		 * @warning This attribute might remove later, if there is no use
		 */
		time_t	_timeConnected;

		/**
		 * @brief Authenicate Level of IRC Client
		 * 
		 * Value that store authenicate levelaccording to IRC Protocol
		 * This Value use bitwise flag to define behaviour
		 * Multiple flags can be combined using the bitwise OR (`|`) operator. 
		 * Possible values are:
		 * - 0x01 (PASS FLAG): Verify Password Authenication
		 * - 0x02 (USER FLAG): Verify Username Authenication
		 * - 0x04 (NICK FLAG): Verify Nickname Authenication
		 * - 0x08 (LOGIN FALG): Verify that this Client is passed all authenication
		 * - 0x16 (DEBUG FLAGE): Use for ignore authenication for debugging purpose
		 * 
		 * @note This Attribute can be ignored if DEBUG flag is setted
		 */
		int	_authenLevel;
		
		/**
		 * @brief UserMode for server level operation
		 * 
		 * @note I have no clue for this attribute, So You can come up with this one
		 */
		std::set<char>	_userMode;

		/**
		 * @brief Queue for RESPONSE messages
		 * 
		 * This Queue used for storing these RESOPONSE messages
		 * Before Sending out in POLLOUT part, one per pollfd loop in FIFO order
		 * 
		 * @note Don't send anything, if this queue is empty
		 */
		std::queue<std::string>	_replyQueue;

		/**
		 * @brief Set of Channel that this Client is membershiped
		 * 
		 * This Set is used for storing Channel Pointer
		 * That this Client is membershiped of
		 * Use for ease of "cleaning" when Client is disconnected
		 * 
		 * @note This attribute migth be use for anything else, but write the comment
		 */
		std::set<Channel*>	_channel;

		/**
		 * @brief Client's Username
		 * 
		 * It can set through USER command
		 * 
		 * @note It can't change, once set
		 */
		std::string	_username;

		/**
		 * @brief Client's Nickname
		 * 
		 * It can set/change through NICK command
		 */
		std::string	_nickname;

		/**
		 * @brief Client's realname
		 * 
		 * It can set through USER command
		 * 
		 * @note It can't change, once set
		 */
		std::string	_realname;

		/**
		 * @brief Client's Host information
		 * 
		 * @note It should be set after client is connected
		 */
		std::string	_host;
};

#endif