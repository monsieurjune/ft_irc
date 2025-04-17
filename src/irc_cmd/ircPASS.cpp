/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircPASS.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 18:33:17 by scharuka          #+#    #+#             */
/*   Updated: 2025/04/17 17:56:57 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "ft_irc/FtIrcFunctionHelper.hpp"

static FtIrc::t_replyBatch	sb_ERR_PASSWDMISMATCH(FtIrc * const obj, Client * const client)
{
	Message	reply_msg;

	// Creating MSG
	reply_msg.setSource(obj->getServerName());
	reply_msg.setCommand(ERR_PASSWDMISMATCH);
	reply_msg.pushParam(client->getNickname().empty() ? "*" : client->getNickname());
	reply_msg.pushParam("Password is incorrected");

	return singleReplySingleClientBatch(reply_msg, client);
}

FtIrc::t_replyBatch	FtIrc::ircPASS(FtIrc * const obj, Message const & msg, Client * const client)
{
	std::vector<std::string> const&	params	= msg.getParams();

	// Invalid size of Params
	if (params.size() != 1)
	{
		return obj->errNeedMoreParams(client, msg);
	}

	// Already Provide PASS
	if (client->containFlags(PASS_FLAG))
	{
		return obj->errAlreadyRegistered(client);
	}

	if (params.at(0) != obj->getServerPassword())
	{
		return sb_ERR_PASSWDMISMATCH(obj, client);
	}

	// Just for avoid confusion in DEBUG MODE
	client->setAuthenLevel(client->getAuthenLevel() | PASS_FLAG);

	return FtIrc::t_replyBatch(); // Return Nothing
}