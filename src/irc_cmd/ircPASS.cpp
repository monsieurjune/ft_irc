/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircPASS.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 18:33:17 by scharuka          #+#    #+#             */
/*   Updated: 2025/02/01 16:53:02 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "ft_irc/ircReply.hpp"

static FtIrc::t_replyBatch	sb_ERR_PASSWDMISMATCH(FtIrc * const obj, Client * const client)
{
	Message	reply_msg;

	// Creating MSG
	reply_msg.setSource(obj->getServerName());
	reply_msg.setCommand(ERR_PASSWDMISMATCH);
	if (client->getNickname().empty())
	{
		reply_msg.pushParam("*");
	}
	else
	{
		reply_msg.pushParam(client->getNickname());
	}
	reply_msg.pushParam("Password is incorrected");

	// Put it in proper struct
	std::queue<Message>	queue;

	queue.push(reply_msg);

	FtIrc::t_reply		reply(client, queue);
	FtIrc::t_replyBatch	batch(1, reply);

	return batch;
}

FtIrc::t_replyBatch	FtIrc::ircPASS(FtIrc * const obj, Message const & msg, Client * const client)
{
	std::vector<std::string> const&	params	= msg.getParams();

	// Invalid size of Params
	if (params.size() != 1)
	{
		return errNeedMoreParams(obj, client, msg.getCommand());
	}

	// Already Provide PASS
	if (client->containFlags(PASS_FLAG))
	{
		return errAlreadyRegistered(obj, client);
	}

	if (params.at(0) != obj->getServerPassword())
	{
		return sb_ERR_PASSWDMISMATCH(obj, client);
	}

	client->setAuthenLevel(PASS_FLAG);

	// Return 0 size vector
	return FtIrc::t_replyBatch();
}