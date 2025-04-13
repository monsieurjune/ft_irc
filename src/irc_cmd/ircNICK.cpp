/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircNICK.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 18:33:17 by scharuka          #+#    #+#             */
/*   Updated: 2025/04/13 23:15:16 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"

static bool	sb_is_nickname_valid(std::string const& nick)
{
	if (nick.length() > NICKLEN)
	{
		return false;
	}

	for (size_t i = 0; i < nick.size(); i++)
	{
		if (nick[i] == '#' || nick[i] == ':' || nick[i] == '*')
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
	if (client->getNickname().empty())
	{
		reply_msg.pushParam("*");
	}
	else
	{
		reply_msg.pushParam(client->getNickname());
	}
	reply_msg.pushParam("No nickname given");

	// Put it in proper struct
	std::queue<Message>	queue;

	queue.push(reply_msg);

	FtIrc::t_reply		reply(client, queue);
	FtIrc::t_replyBatch	batch(1, reply);

	return batch;
}

static FtIrc::t_replyBatch	sb_ERRONEUSNICKNAME(FtIrc * const obj, Client * const client, std::string const& new_nick)
{
	Message	reply_msg;

	// Creating MSG
	reply_msg.setSource(obj->getServerName());
	reply_msg.setCommand(ERR_ERRONEUSNICKNAME);
	if (client->getNickname().empty())
	{
		reply_msg.pushParam("*");
	}
	else
	{
		reply_msg.pushParam(client->getNickname());
	}
	reply_msg.pushParam(new_nick);
	reply_msg.pushParam("Erroneus nickname");

	// Put it in proper struct
	std::queue<Message>	queue;

	queue.push(reply_msg);

	FtIrc::t_reply		reply(client, queue);
	FtIrc::t_replyBatch	batch(1, reply);

	return batch;
}

static FtIrc::t_replyBatch	sb_ERR_NICKNAMEINUSE(FtIrc * const obj, Client * const client, std::string const& new_nick)
{
	Message	reply_msg;

	// Creating MSG
	reply_msg.setSource(obj->getServerName());
	reply_msg.setCommand(ERR_NICKNAMEINUSE);
	if (client->getNickname().empty())
	{
		reply_msg.pushParam("*");
	}
	else
	{
		reply_msg.pushParam(client->getNickname());
	}
	reply_msg.pushParam(new_nick);
	reply_msg.pushParam("Nickname is already in use");

	// Put it in proper struct
	std::queue<Message>	queue;

	queue.push(reply_msg);

	FtIrc::t_reply		reply(client, queue);
	FtIrc::t_replyBatch	batch(1, reply);

	return batch;
}

static FtIrc::t_replyBatch	sb_authen(FtIrc * const obj, Message const & msg, Client * const client)
{
	std::vector<std::string> const&	params		= msg.getParams();
	std::string						old_nick	= client->getNickname();
	std::string const&				new_nick	= params.at(0);
	Client							*tmpclient	= obj->getClientByNickname(new_nick);

	if (tmpclient != NULL)
	{
		if (tmpclient == client)
		{
			return FtIrc::t_replyBatch(); // IRC won't response and let client to "handle"
		}
		return sb_ERR_NICKNAMEINUSE(obj, client, new_nick);
	}

	// Set Nickname To Client
	obj->changeClientNickname(old_nick, new_nick);

	// Retrieve
	// TODO: beware of mid change nickname while authen too

	return FtIrc::t_replyBatch(); // TODO: fix this line later
}

static FtIrc::t_replyBatch	sb_non_authen(FtIrc * const obj, Message const & msg, Client * const client)
{
	std::vector<std::string> const&	params	= msg.getParams();
	std::string const&	new_nick			= params.at(0);

	if (obj->getClientByNickname(new_nick) != NULL)
	{
		return sb_ERR_NICKNAMEINUSE(obj, client, new_nick);
	}

	// Set Nickname To Client
	obj->setClientNickname(client, new_nick);
	client->setAuthenLevel(client->getAuthenLevel() | USER_FLAG);

	if (client->containFlags(USER_FLAG | PASS_FLAG | NICK_FLAG))
	{
		// TODO: Handle the greeting
	}

	return FtIrc::t_replyBatch();
}

FtIrc::t_replyBatch	FtIrc::ircNICK(FtIrc * const obj, Message const & msg, Client * const client)
{
	std::vector<std::string> const&	params	= msg.getParams();

	if (params.size() < 1)
	{
		return sb_ERR_NONICKNAMEGIVEN(obj, client);
	}

	std::string const&	new_nick	= params.at(0);

	if (params.size() == 1 && msg.hasTrailing())
	{
		return sb_ERRONEUSNICKNAME(obj, client, "*");
	}

	if (!sb_is_nickname_valid(new_nick))
	{
		return sb_ERRONEUSNICKNAME(obj, client, "*");
	}

	if (client->containFlags(NICK_FLAG))
	{
		return sb_authen(obj, msg, client);
	}
	else
	{
		return sb_non_authen(obj, msg, client);
	}
}