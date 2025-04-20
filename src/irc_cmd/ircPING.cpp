/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircPING.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 18:33:17 by scharuka          #+#    #+#             */
/*   Updated: 2025/04/20 12:12:10 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "ft_irc/FtIrcFunctionHelper.hpp"

FtIrc::t_replyBatch	FtIrc::ircPING(FtIrc * const obj, Message const & msg, Client * const client)
{
	std::vector<std::string> const&	params	= msg.getParams();

	// Invalid size of Params
	if (params.size() != 1)
	{
		return obj->errNeedMoreParams(client, msg);
	}

	Message	reply_msg;

	// Creating MSG
	reply_msg.setSource(obj->getServerName());
	reply_msg.setCommand("PONG");
	reply_msg.pushParam(obj->getServerName());
	reply_msg.pushParam(params.at(0));

	return singleReplySingleClientBatch(reply_msg, client);
}