/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 01:57:58 by tnualman          #+#    #+#             */
/*   Updated: 2024/12/31 20:50:08 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FT_IRC_HPP__
#define __FT_IRC_HPP__

// Project Header
#include "FtIrcConstants.hpp"
#include "Message.hpp"
#include "Client.hpp"
#include "Channel.hpp"

// C Header
#include <poll.h>
#include <ctime>

// CPP Header
#include <vector>
#include <map>
#include <set>
#include <string>

// Debug Mode
#ifndef DEBUG_MODE
#define DEBUG_MODE 1
#endif

/**
 * @class FtIrc
 * @brief A Class that use for 
 * 
 * - handling IRC CMD
 * 
 * - dynamic allocate Client & CHaannel
 * 
 * @warning There must be only 1 class for entire program (no deep copy)
 * 
 * @note If you want to parse this class to other function, use pointer or reference
 */
class FtIrc
{
	public:
	
		typedef std::pair<Client *, std::queue<Message> >	t_reply;
		typedef std::vector<t_reply>						t_replyBatch;
		typedef t_replyBatch (*t_IrcCmd)(Message const &, Client * const);
	
	private:
		/**
		 * @brief Client Map that use its fd as key to Client Pointer
		 * 
		 * @note This is main place where store heap of Client
		 */
		std::map<int, Client*>	_clientMapByFd;

		/**
		 * @brief Client Map that use its nickname as key to Client Pointer
		 */
		std::map<std::string, Client*>	_clientMapByNickname;

		/**
		 * @brief Channel Map that use its name as key to Channel Pointer
		 * 
		 * @note This is main place where store heap of Channel
		 */
		std::map<std::string, Channel*>	_channelMapByName;

		/**
		 * @brief Array of PollFd in Vector from for ease of use
		 */
		std::vector<struct pollfd>	_pollfdVec;

		/**
		 * @brief Server Name
		 * 
		 * @note It use as Prefix for Server Response
		 */
		std::string const	_serverName;

		/**
		 * @brief Server Passsword
		 */
		std::string const	_serverPassword;

		/**
		 * @brief Time when Server is stared
		 * 
		 * @warning If it might not be used, it would be remove later
		 */
		std::time_t const	_timeServerStarted;

		/**
		 * @brief Map of IRC CMD that doesn't required client to 
		 * provide password before use them (i.e. PASS, CAP)
		 */
		std::map<std::string, t_IrcCmd>	_noPassCmdMap;

		/**
		 * @brief Map of IRC CMD that use for authenication after
		 * provide correct password (i.e. NICK, PASS)
		 */
		std::map<std::string, t_IrcCmd>	_authenCmdMap;

		/**
		 * @brief Map of IRC CMD that need authenication to completed 
		 * before use them (literally majority of cmd)
		 */
		std::map<std::string, t_IrcCmd>	_normalCmdMap;

		/**
		 * @brief Create Channel
		 * 
		 * This method is where Channel allocation and storage management occrued
		 * 
		 * @param channel_name New Channel Name
		 * @param creator Client that request for creating CHannel
		 * 
		 * @warning This method should not work with unvalidate data, despite it has validation
		 * @note Should be called under JOIN
		 * 
		 * @return Integer that indicate success
		 * 
		 * - 0: success
		 * 
		 * - 1: already has this channel
		 * 
		 * - -1: allocation failed
		 */
		int	createChannel(std::string const channel_name, Client * const creator);

		/**
		 * @brief Unjoin certain Client from Channel
		 * 
		 * There are 2 possible outcome for Channel:
		 * 
		 * - member_after_removed > 0: Nothing Happend for Channel
		 * 
		 * - member_after_removed = 0: Channel is deleted
		 * 
		 * @param channel_name Channel Name that certain client want to unjoin
		 * @param client Client that want to unjoin
		 * 
		 * @warning This method should not work with unvalidate data, despite it has validation
		 * @note Should be called under PART, KICK, QUIT and unexpected quit case
		 * 
		 * @return Integer that indicate success
		 * 
		 * - 0: success
		 * 
		 * - 1: already has this channel
		 * 
		 */
		int	deleteClientFromChannel(std::string const channel_name, Client * const client);

	public:
		/**
		 * @brief Construct an FtIrc Object
		 * 
		 * @param password IRC Server's Password
		 * @param servername IRC Server's Name (use for server's prefix)
		 */
		FtIrc(std::string const password, std::string const servername);

		/**
		 * @brief Default Destructor
		 */
		~FtIrc();

		/**
		 * @brief Add new Client Object by its socket fd
		 * 
		 * @param fd new socket fd from accept()
		 */
		void	addClient(int const fd);

		/**
		 * @brief Delete This Client Object by its socket fd
		 * 
		 * @param fd socket fd that disconnect or call QUIT
		 */
		void	deleteClient(int const fd);

		/**
		 * @brief Get Client by its fd
		 * 
		 * @param fd This Client's fd
		 * 
		 * @return Client Object as pointer (mustn't call delete on it)
		 */
		Client*	getClientByFd(int const fd)	const;

		/**
		 * @brief Get Client by its nickname
		 * 
		 * @param fd This Client's Nickname
		 * 
		 * @return Client Object as pointer (mustn't call delete on it)
		 */
		Client*	getClientByNickname(std::string const nick)	const;

		/**
		 * @brief Get Channel by its Name
		 * 
		 * @param fd This Channel's Name
		 * 
		 * @return Channel Object as pointer (mustn't call delete on it)
		 */
		Channel*	getChannelByName(std::string const name)	const;

		/**
		 * @brief Get PollFd array
		 * 
		 * @return pollfd array as pointer to its vector
		 * 
		 * @note Only use for calling poll() and nothing else
		 */
		const struct pollfd*	getPollFd()	const;

		/**
		 * @brief IRC CMD Main Handler
		 * 
		 * It's working as following procedure
		 * 
		 * - Parsing CMD to its associate method
		 * 
		 * - Get vector of reply Message Objects of associate clients
		 * 
		 * - Convert Messages to String before enqueue them to associate clients
		 * 
		 * @param msg Message Object that parse from client
		 * @param client Client Object of Client that send the msg
		 */
		void	ircMessageHandler(Message const & msg, Client * const client);
};

#endif