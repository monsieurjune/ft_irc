/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errInputTooLong.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 05:45:40 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/20 11:58:40 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "ft_irc/FtIrcFunctionHelper.hpp"

FtIrc::t_replyBatch	FtIrc::errInputTooLong(Client * const client)
{
	Message	reply_msg;

	// Creating MSG
	reply_msg.setSource(_serverName);
	reply_msg.setCommand(ERR_INPUTTOOLONG);
	reply_msg.pushParam(client->getNickname().empty() ? "*" : client->getNickname());
	reply_msg.pushParam("Input line was too long");

	return singleReplySingleClientBatch(reply_msg, client);
}