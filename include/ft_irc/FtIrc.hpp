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

class FtIrc
{
	private:
		typedef std::pair<Client *, std::queue<Message> >	t_reply;
		typedef std::vector<t_reply>						t_replyBatch;
		typedef t_replyBatch (*t_IrcCmd)(Message const &, Client const * const);

		/**
		 * @brief Set of Client
		 * 
		 * @note This is only place to dynamic allocate & store Client
		 */
		std::set<Client>	_clientSet;

		/**
		 * @brief Set of Channel
		 * 
		 * @note This is only place to dynamic allocate & store Channel
		 */
		std::set<Channel>	_channelSet;

		/**
		 * @brief Client Map that use its fd as key
		 */
		std::map<int, Client*>	_clientMapByFd;

		/**
		 * @brief Client Map that use its nickname as key
		 */
		std::map<std::string, Client*>	_clientMapByNickname;

		/**
		 * @brief Channel Map that use its name as key
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
		time_t const	_timeServerStarted;

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
};

#endif