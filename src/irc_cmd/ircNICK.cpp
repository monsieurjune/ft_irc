/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircNICK.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 18:33:17 by scharuka          #+#    #+#             */
/*   Updated: 2025/04/15 15:20:12 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "ft_irc/FtIrcFunctionHelper.hpp"

static inline bool	sb_is_nickname_valid(std::string const& nick)
{
	if (nick.length() > NICKLEN)
	{
		return false;
	}

	for (size_t i = 0; i < nick.size(); i++)
	{
		if (nick[i] == ' ' || nick[i] == '#' || nick[i] == ':' || nick[i] == '*')
		{
			return false;
		}
	}
	return true;
}

static FtIrc::t_replyBatch	sb_ERR_NONICKNAMEGIVEN(FtIrc * const obj, Client * const client)
{
	Message	reply_msg;

	// Creating MSG
	reply_msg.setSource(obj->getServerName());
	reply_msg.setCommand(ERR_NONICKNAMEGIVEN);
	nicknameMessageHelper(reply_msg, client);
	reply_msg.pushParam("No nickname given");

	return singleReplySingleClientBatch(reply_msg, client);
}

static FtIrc::t_replyBatch	sb_ERRONEUSNICKNAME(FtIrc * const obj, Client * const client, std::string const& new_nick)
{
	Message	reply_msg;

	// Creating MSG
	reply_msg.setSource(obj->getServerName());
	reply_msg.setCommand(ERR_ERRONEUSNICKNAME);
	nicknameMessageHelper(reply_msg, client);
	reply_msg.pushParam(new_nick);
	reply_msg.pushParam("Erroneus nickname");

	return singleReplySingleClientBatch(reply_msg, client);
}

static FtIrc::t_replyBatch	sb_ERR_NICKNAMEINUSE(FtIrc * const obj, Client * const client, std::string const& new_nick)
{
	Message	reply_msg;

	// Creating MSG
	reply_msg.setSource(obj->getServerName());
	reply_msg.setCommand(ERR_NICKNAMEINUSE);
	nicknameMessageHelper(reply_msg, client);
	reply_msg.pushParam(new_nick);
	reply_msg.pushParam("Nickname is already in use");

	return singleReplySingleClientBatch(reply_msg, client);
}

static FtIrc::t_replyBatch	sb_authen(FtIrc * const obj, Message const & msg, Client * const client)
{
	std::vector<std::string> const&	params		= msg.getParams();
	std::string						old_nick	= client->getNickname();
	std::string const&				new_nick	= params.at(0);
	std::string						old_source	= client->constructSource();

	// Set Nickname To Client
	obj->changeClientNickname(old_nick, new_nick);

	// in case of mid change nickname during authen process
	if (!client->containFlags(LOGIN_FLAG))
	{
		return FtIrc::t_replyBatch(); // Return Nothing
	}

	// Retrieve All Client that share same channels with this client
	std::set<Channel*>	channelSet = obj->getChannelSetByClient(client);
	std::set<Client*>	clientSet = obj->getClientSetByChannelSet(channelSet);

	// publish to all client
	Message	reply_msg;

	reply_msg.setSource(old_source);
	reply_msg.setCommand("NICK");
	reply_msg.pushParam(new_nick);

	return singleReplyMultiClientBatch(reply_msg, clientSet);
}

static inline void	sb_non_authen(FtIrc * const obj, Message const & msg, Client * const client)
{
	std::vector<std::string> const&	params	= msg.getParams();
	std::string const&	new_nick			= params.at(0);

	// Set Nickname To Client
	obj->setClientNickname(client, new_nick);
	client->setAuthenLevel(client->getAuthenLevel() | USER_FLAG);
}

// <nickname> [ <hopcount> ]

FtIrc::t_replyBatch	FtIrc::ircNICK(FtIrc * const obj, Message const & msg, Client * const client)
{
	std::vector<std::string> const&	params	= msg.getParams();

	if (params.size() < 1)
	{
		return sb_ERR_NONICKNAMEGIVEN(obj, client);
	}

	// Either return ERRONEUSNICKNAME or NOTENOUGHPARAM
	// The second parameter is ignored if client is send (not server)
	if (params.size() > 2)
	{
		return obj->errNeedMoreParams(client, msg);
	}

	std::string const&	new_nick	= params.at(0);
	Client				*tmpclient	= NULL;

	// Invalid Character
	if (!sb_is_nickname_valid(new_nick))
	{
		return sb_ERRONEUSNICKNAME(obj, client, new_nick);
	}

	// Duplicate Nickname
	tmpclient = obj->getClientByNickname(new_nick);
	if (tmpclient != NULL)
	{
		// Do Nothing, if client just send same name
		if (tmpclient == client)
		{
			return FtIrc::t_replyBatch();
		}
		return sb_ERR_NICKNAMEINUSE(obj, client, new_nick);
	}

	if (client->containFlags(NICK_FLAG))
	{
		return sb_authen(obj, msg, client);
	}
	else
	{
		sb_non_authen(obj, msg, client);

		if (client->containFlags(USER_FLAG | PASS_FLAG | NICK_FLAG))
		{
			return obj->rplWelcome(client);
		}
		return FtIrc::t_replyBatch(); // Do Nothing
	}
}