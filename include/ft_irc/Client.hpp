/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:31:35 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/09 12:51:16 by tponutha         ###   ########.fr       */
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

// Debug Mode

#ifndef DEBUG_MODE
#define DEBUG_MODE true
#endif

// Name Length
#ifndef USERLEN
#define USERLEN 10
#endif

#ifndef NICKLEN
#define NICKLEN 9
#endif

// Authenication Flag

#ifndef PASS_FLAG
#define PASS_FLAG 0x01
#endif

#ifndef USER_FLAG
#define USER_FLAG 0x02
#endif

#ifndef NICK_FLAG
#define NICK_FLAG 0x04
#endif

#ifndef LOGIN_FLAG
#define LOGIN_FLAG 0x08
#endif

#ifndef CAP_FLAG
#define CAP_FLAG 0x10
#endif

#ifndef DEBUG_FLAG
#define DEBUG_FLAG 0x80
#endif

// Forward Decalration to avoid CIRCULAR include
class Channel;

/**
 * @class Client
 * @brief A Class For Encapsulate IRC Client
 * 
 * This Class is Use for Encapsulate IRC Socket and Its assosiated properties.
 */
class Client
{
	private:
		/**
		 * @brief The socket's fd of Client
		 * 
		 * @note Only close it in deconstructor's scope
		 */
		int const	_fd;
		
		/**
		 * @brief Time when client is connected
		 * 
		 * @note It should be time when Client is constructed
		 * @warning This attribute might remove later, if there is no use
		 */
		std::time_t	const	_timeConnected;

		/**
		 * @brief Authenicate Level of IRC Client
		 * 
		 * Value that store authenicate level according to IRC Protocol
		 * and use bitwise flag to define behaviour
		 * 
		 * Multiple flags can be combined using the bitwise OR (`|`) operator.
		 * 
		 * Possible values are:
		 * 
		 * - 0x01 (PASS FLAG): Verify Password Authenication
		 * 
		 * - 0x02 (USER FLAG): Verify Username Authenication
		 * 
		 * - 0x04 (NICK FLAG): Verify Nickname Authenication
		 * 
		 * - 0x08 (LOGIN FALG): Verify that this Client is passed all authenication
		 * 
		 * - 0x10 (CAP FLAG): Use for Checking if Client sent CAP LS
		 * 
		 * - 0x80 (DEBUG FLAG): Use for overide authenication for debugging purpose
		 * 
		 * @note This Attribute can be ignored if DEBUG flag is setted
		 */
		int	_authenLevel;
		
		/**
		 * @brief UserMode for server level operation
		 * 
		 * @note For the minimum requirement, the only mode to implement is operator ('o').
		 */
		std::set<char>	_userMode;

		/**
		 * @brief Queue for RESPONSE messages
		 * 
		 * This Queue used for storing these RESOPONSE messages,
		 * before Sending out in POLLOUT part, one per pollfd loop in FIFO order
		 * 
		 * @warning 
		 * - Should dequeue out and send one time per poll loop
		 * 
		 * - Don't send anything, if this queue is empty
		 */
		std::queue<std::string>	_replyQueue;

		/**
		 * @brief Client's Username
		 * 
		 * @note It can set through USER command
		 * 
		 * @warning It mustn't be changed, once set.
		 */
		std::string	_username;

		/**
		 * @brief Client's Nickname
		 * 
		 * @note It can set/change through NICK command
		 */
		std::string	_nickname;

		/**
		 * @brief Client's realname
		 * 
		 * @note It can set through USER command
		 * 
		 * @warning It mustn't be changed, once set.
		 */
		std::string	_realname;

		/**
		 * @brief Client's Host information
		 * 
		 * @note It should be set after client is connected
		 */
		std::string	_host;

	public:
		/**
		 * @brief Constructs an Client object
		 * 
		 * @param fd This Object's socket fd
		 */
		Client(int const fd);
		
		/**
		 * @brief Default Destructor
		 */
		~Client();

		/**
		 * @brief Get This Client's socket fd
		 * 
		 * @return This Client's socket fd
		 * 
		 * @note Do not attempt to close this fd outside of Destructor's scope
		 */
		int	getFd()	const;

		/**
		 * @brief Get This Client's time when connected
		 * 
		 * @return Time when This Client is constructed
		 */
		std::time_t	getTimeConnected()	const;

		/**
		 * @brief Get This Client's Authenication Level
		 * 
		 * @return Integer that Indicate authenication level
		 * 
		 * @note Use bitwise to extract authenicate flag
		 */
		int	getAuthenLevel()	const;

		/**
		 * @brief Get This Client's Nickname
		 * 
		 * @return This Client's Nickname as lvalue
		 */
		std::string const&	getNickname()	const;

		/**
		 * @brief Get This Client's Username
		 * 
		 * @return This Client's Username as lvalue
		 */
		std::string const&	getUsername()	const;

		/**
		 * @brief Get This Client's Realname
		 * 
		 * @return This Client's Realname as lvalue
		 */
		std::string const&	getRealname()	const;

		/**
		 * @brief Get This Client's Host Information
		 * 
		 * @return This Client's Host Information as lvalue
		 */
		std::string const&	getHost()	const;

		/**
		 * @brief Get This Client's Membership Mode
		 * 
		 * @return Set of Charactors that represent membership mode
		 */
		std::set<char> const&	getModes()	const;

		/**
		 * @brief Check Flags combination of This Client
		 * 
		 * Multiple flags can be combined using the bitwise OR (`|`) operator.
		 * 
		 * Possible values are:
		 * 
		 * - 0x01 (PASS FLAG): Verify Password Authenication
		 * 
		 * - 0x02 (USER FLAG): Verify Username Authenication
		 * 
		 * - 0x04 (NICK FLAG): Verify Nickname Authenication
		 * 
		 * - 0x08 (LOGIN FALG): Verify that this Client is passed all authenication
		 * 
		 * - 0x10 (CAP FLAG): Use for Checking if Client sent CAP LS
		 * 
		 * - 0x80 (DEBUG FLAG): Use for overide authenication for debugging purpose
		 */
		bool	containFlags(int flags)	const;

		/**
		 * @brief Check if This Client contain certain mode
		 * 
		 * @return True if contain this mode, false otherwise
		 */
		bool	hasMode(char c)	const;

		/**
		 * @brief Creating Client's Source
		 * 
		 * @return Client's Source in format: <nick>!<user>@<ip>
		 */
		std::string	constructSource();

		/**
		 * @brief Set This Client's authenication level
		 * 
		 * Multiple flags can be combined using the bitwise OR (`|`) operator.
		 * 
		 * Possible values are:
		 * 
		 * - 0x01 (PASS FLAG): Verify Password Authenication
		 * 
		 * - 0x02 (USER FLAG): Verify Username Authenication
		 * 
		 * - 0x04 (NICK FLAG): Verify Nickname Authenication
		 * 
		 * - 0x08 (LOGIN FALG): Verify that this Client is passed all authenication
		 * 
		 * - 0x10 (CAP FLAG): Use for Checking if Client sent CAP LS
		 * 
		 * - 0x80 (DEBUG FLAG): Use for overide authenication for debugging purpose
		 * 
		 * @param level Authenication Level
		 * 
		 * @note This Method is totally remove old one. If you want to 'append' new flag.
		 * You can get old value and 'bitwise OR' to new value
		 */
		void	setAuthenLevel(int const level);

		/**
		 * @brief Set This Client's Nickname
		 * 
		 * @param name Nickname
		 * 
		 * @warning It must be called in NICK's fucntion
		 */
		void	setNickname(std::string const name);

		/**
		 * @brief Set This Client's Username
		 * 
		 * @param name Username
		 * 
		 * @warning It must be called in USER's function
		 */
		void	setUsername(std::string const name);

		/**
		 * @brief Set This Client's Realname
		 * 
		 * @param name Realname
		 * 
		 * @warning It must be called in USER's function
		 */
		void	setRealname(std::string const name);

		/**
		 * @brief Set This Client's Host Information
		 * 
		 * @param host Host Information
		 * 
		 * @warning It must be called after Client is built
		 */
		void	setHost(std::string const host);

		/**
		 * @brief Add This Membership Mode to This Client
		 * 
		 * @param c Membership Character
		 */
		void	addMode(char const c);

		/**
		 * @brief Remove This Membership Mode from This Client
		 * 
		 * @param c Membership Charactor
		 */
		void	removeMode(char const c);

		/**
		 * @brief Get Amount of Message in Queue
		 * 
		 * @return Amount of Message
		 */
		size_t	countReply()	const;

		/**
		 * @brief Enqueue This Message
		 * 
		 * @param msg IRC Response Message
		 */
		void	enqueueReply(std::string const msg);

		/**
		 * @brief Dequeue This Message
		 * 
		 * @return IRC Response Message in rvalue
		 * 
		 * @warning Only Use it in function that send msg out
		 */
		std::string dequeueReply();
};

#endif