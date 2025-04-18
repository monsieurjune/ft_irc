/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 01:57:58 by tnualman          #+#    #+#             */
/*   Updated: 2025/04/19 03:16:30 by tponutha         ###   ########.fr       */
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
		typedef t_replyBatch (*t_IrcCmd)(FtIrc * const, Message const &, Client * const);
	
	private:
		/**
		 * @brief Server's Listener socket fd
		 * 
		 * @note It must be created via listen() with proper bind()
		 */
		int const	_listen_fd;

		/**
		 * @brief Server Name
		 * 
		 * @note It use as Prefix for Server Response
		 */
		std::string const	_serverName;

		/**
		 * @brief Network Name
		 * 
		 * @note It's only matter when implement server-to-server communication
		 */
		std::string const	_networkName;

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
		 * @brief Main Array of PollFd in Vector form for ease of use
		 */
		std::vector<struct pollfd>	_mainPollfdVec;

		/**
		 * @brief Temporary Vector for cleaning disconected fd
		 */
		std::vector<struct pollfd>	_tempPollfdVec;

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

	public:
		/**
		 * @brief Construct an FtIrc Object
		 * 
		 * @param listen_fd IRC Server's Listen socket fd
		 * @param servername IRC Server's Name (use for server's prefix)
		 * @param networkname IRC Server's Name (use for server's prefix)
		 * @param password IRC Server's Password
		 */
		FtIrc(
			int const listen_fd, 
			std::string const& servername,
			std::string const& networkname,
			std::string const& password
		);

		/**
		 * @brief Default Destructor
		 */
		~FtIrc();

		/**
		 * @brief Add new Client Object by its socket fd
		 * 
		 * @param fd new socket fd from accept()
		 * @param ip Ip Address of Client in cstring
		 */
		void	addClient(int const fd, const char *ip);

		/**
		 * @brief Delete This Client Object by its socket fd
		 * 
		 * @param fd socket fd that disconnect or call QUIT
		 */
		void	deleteClient(int const fd);

		/**
		 * @brief Get Listener Socket Fd
		 * 
		 * @return Listenner Socket Fd
		 * 
		 * @warning Don't close it outside of class at all
		 */
		int	getListenFd()	const;

		/**
		 * @brief Get Server's Name
		 * 
		 * @return Server Name as lvalue
		 */
		std::string const&	getServerName()	const;

		/**
		 * @brief Get Network Name
		 * 
		 * @return Network Name as lvalue
		 */
		std::string const&	getNetworkName() const;

		/**
		 * @brief Get Server Password
		 * 
		 * @return Server Password as lvalue
		 */
		std::string const&	getServerPassword()	const;

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
		 * @param nick This Client's Nickname
		 * 
		 * @return Client Object as pointer (mustn't call delete on it)
		 */
		Client*	getClientByNickname(std::string const& nick)	const;

		/**
		 * @brief Get Channel by its Name
		 * 
		 * @param name This Channel's Name
		 * 
		 * @return Channel Object as pointer (mustn't call delete on it)
		 */
		Channel*	getChannelByName(std::string const& name)	const;

		/**
		 * @brief Get PollFd array to work with poll()
		 * 
		 * @return vector of main pollfd as lvalue
		 */
		std::vector<struct pollfd>&	getPollFdVector();

		/**
		 * @brief Create the set of Channel that this client is joining
		 * 
		 * @param client Target Client
		 * 
		 * @return Set of Channel Pointer
		 */
		std::set<Channel*>	getChannelSetByClient(Client * const client) const;

		/**
		 * @brief Create the set of Client that is joining this channel
		 * 
		 * @param channel Target Channel
		 * 
		 * @return Set of Client Pointer
		 */
		std::set<Client*>	getClientSetByChannel(Channel * const channel) const;

		/**
		 * @brief Create the set of Client that is joining these channels
		 * 
		 * @param channelSet Channel Set
		 * 
		 * @return Set of Client Pointer
		 */
		std::set<Client*>	getClientSetByChannelSet(std::set<Channel*> const& channelSet) const;

		/**
		 * @brief Set Client's Nickname
		 * 
		 * @param old_nick Old Client's Nickname
		 * @param new_nick New Client's Nickname
		 * 
		 * @note It will ignored, if new_nick is already existed or old_nick isn't existed
		 */
		void	changeClientNickname(std::string const& old_nick, std::string const& new_nick);

		/**
		 * @brief Set Client Object to Map with its Nickname
		 * 
		 * @param client Client Pointer
		 * @param nick Client's Nickname
		 * 
		 * @warning Only Use for register new client that provided nickname in authenication
		 */
		void	setClientNickname(Client * const client, std::string const& nick);

		/**
		 * @brief Clean up the "disconnected" pollfd from vector
		 * 
		 * This method will remove all fd that marked by deleteClient()
		 * 
		 * @note This method existed to avoid O(N^2) of constant erase()
		 * 
		 * @warning This method must be called after pollfd loop is ended with these reason
		 * 
		 * - avoid "skipping" the index in loop
		 * 
		 * - avoid poll() to interact with invalid fd (marked fd)
		 * 
		 * - this method "swap" the original pollfd, so update the reference after that
		 */
		void	cleanUnusedPollFd();

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
		int	createChannel(std::string const& channel_name, Client * const creator);

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
		 * - 1: channel doesn't exist
		 * 
		 * - 2: client doesn't exist in this channel
		 */
		int	deleteClientFromChannel(std::string const& channel_name, Client * const client);

		/**
		 * @brief Push Message in Reply Batch for all Clients in this Channel
		 * 
		 * @param reply_msg Message that want to send
		 * @param channel Target Channel that want to broadcast
		 * @param batch Reply Batch Data Structure
		 * 
		 * @warning Don't use this method in scenario that client joined multiple channels
		 */
		void	pushChannelReplyAll(Message const & reply_msg, Channel * const channel, t_replyBatch & batch);

		/**
		 * @brief Push Message in Reply Batch for all Clients in server
		 * 
		 * @param reply_msg Message that want to send
		 * @param batch Reply Batch Data Structure
		 */
		void	pushServerReplyAll(Message const & reply_msg, t_replyBatch & batch);

	private:
		/**
		 * @brief Transform and Put Message to Client's Storage
		 * 
		 * @param batch Group of Reply Message
		 */
		void	applyReplyBatchToClient(t_replyBatch& batch);

		/**
		 * @brief IRC CMD KICK
		 * 
		 * @param obj FtIrc Object
		 * @param msg Message Object
		 * @param client Client Object
		 * 
		 * @return Vector of reply Message Objects of associate clients
		 */
		static t_replyBatch ircKICK(FtIrc * const obj, Message const & msg, Client * const client);

		/**
		 * @brief IRC CMD INVITE
		 * 
		 * @param obj FtIrc Object
		 * @param msg Message Object
		 * @param client Client Object
		 * 
		 * @return Vector of reply Message Objects of associate clients
		 */
		static t_replyBatch ircINVITE(FtIrc * const obj, Message const & msg, Client * const client);

		/**
		 * @brief IRC CMD TOPIC
		 * 
		 * @param obj FtIrc Object
		 * @param msg Message Object
		 * @param client Client Object
		 * 
		 * @return Vector of reply Message Objects of associate clients
		 */
		static t_replyBatch ircTOPIC(FtIrc * const obj, Message const & msg, Client * const client);

		/**
		 * @brief IRC CMD MODE
		 * 
		 * @param obj FtIrc Object
		 * @param msg Message Object
		 * @param client Client Object
		 * 
		 * @return Vector of reply Message Objects of associate clients
		 */
		static t_replyBatch ircMODE(FtIrc * const obj, Message const & msg, Client * const client);

		/**
		 * @brief 
		 * 
		 * @param obj FtIrc Object
		 * @param msg Message Object
		 * @param client Client Object
		 * 
		 * @return Vector of reply Message Objects of associate clients
		 */
		static t_replyBatch ircJOIN(FtIrc * const obj, Message const & msg, Client * const client);

		/**
		 * @brief IRC CMD PRIVMSG
		 * 
		 * @param obj FtIrc Object
		 * @param msg Message Object
		 * @param client Client Object
		 *  
		 * @return Vector of reply Message Objects of associate clients
		 */
		static t_replyBatch ircPRIVMSG (FtIrc * const obj, Message const & msg, Client * const client);

		/**
		 * @brief IRC CMD PASS
		 * 
		 * @param obj FtIrc Object
		 * @param msg Message Object
		 * @param client Client Object
		 * 
		 * @return Vector of reply Message Objects of associate clients
		 */
		static t_replyBatch ircPASS(FtIrc * const obj, Message const & msg, Client * const client);

		/**
		 * @brief IRC CMD NICK
		 * 
		 * @param obj FtIrc Object
		 * @param msg Message Object
		 * @param client Client Object
		 * 
		 * @return Vector of reply Message Objects of associate clients
		 */
		static t_replyBatch ircNICK(FtIrc * const obj, Message const & msg, Client * const client);

		/**
		 * @brief IRC CMD USER
		 * 
		 * @param obj FtIrc Object
		 * @param msg Message Object
		 * @param client Client Object
		 * 
		 * @return Vector of reply Message Objects of associate clients
		 */
		static t_replyBatch ircUSER(FtIrc * const obj, Message const & msg, Client * const client);

		/**
		 * @brief IRC CMD CAP
		 * 
		 * @param obj FtIrc Object
		 * @param msg Message Object
		 * @param client Client Object
		 * 
		 * @return Vector of reply Message Objects of associate clients
		 */
		static t_replyBatch ircCAP(FtIrc * const obj, Message const & msg, Client * const client);

		/**
		 * @brief IRC CMD PART
		 * 
		 * @param obj FtIrc Object
		 * @param msg Message Object
		 * @param client Client Object
		 * 
		 * @return Vector of reply Message Objects of associate clients
		 */
		static t_replyBatch ircPART(FtIrc * const obj, Message const & msg, Client * const client);

		/**
		 * @brief IRC CMD QUIT
		 * 
		 * @param obj FtIrc Object
		 * @param msg Message Object
		 * @param client Client Object
		 * 
		 * @return Vector of reply Message Objects of associate clients
		 */
		static t_replyBatch ircQUIT(FtIrc * const obj, Message const & msg, Client * const client);

		/**
		 * @brief IRC CMD PING
		 * 
		 * @param obj FtIrc Object
		 * @param msg Message Object
		 * @param client Client Object
		 * 
		 * @return Vector of reply Message Objects of associate clients
		 */
		static t_replyBatch ircPING(FtIrc * const obj, Message const & msg, Client * const client);

		/**
		 * @brief IRC CMD PING
		 * 
		 * @param obj FtIrc Object
		 * @param msg Message Object
		 * @param client Client Object
		 * 
		 * @return Vector of reply Message Objects of associate clients
		 */
		static t_replyBatch ircPONG(FtIrc * const obj, Message const & msg, Client * const client);

		/**
		 * 
		 */
		t_replyBatch	ircMODE_channel(Message const & message, Client * const sender);

		/**
		 * 
		 */
		t_replyBatch	ircMODE_user(Message const & message, Client * const sender);

		/**
		 * 
		 */
		t_replyBatch	errNeedMoreParams(Client * const client, Message const & message);

		/**
		 * 
		 */
		t_replyBatch	errUnknownCmd(Client * const client, std::string const& cmd);

		/**
		 * 
		 */
		t_replyBatch	errAlreadyRegistered(Client * const client);

		/**
		 * 
		 */
		t_replyBatch	errNotRegistered(Client * const client);

		/**
		 * 
		 */
		t_replyBatch	errInputTooLong(Client * const client);

		/**
		 * 
		 */
		t_replyBatch	rplTopicWhoTime(Client * const sender, Channel * const channel);

		/**
		 * 
		 */
		t_replyBatch	rplNameReply(Client * const client, Channel * const channel);

		/**
		 * 
		 */
		t_replyBatch	rplWelcome(Client * const client);

		/**
		 * 
		 */
		t_replyBatch	rplWhoReply(Channel * const channel);
};

#endif