/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errNeedMoreParams.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 05:44:56 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/14 04:29:56 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "std/ft_cppstd.hpp"

FtIrc::t_replyBatch	FtIrc::errNeedMoreParams(Client * const client, Message const & message)
{
	Message	reply_msg;

	// Creating MSG
	reply_msg.setSource(_serverName);
	reply_msg.setCommand(ERR_NEEDMOREPARAMS);
	nicknameMessageHelper(reply_msg, client);
	reply_msg.pushParam(message.getCommand());
	reply_msg.pushParam("Not enough parameters");

	return singleReplyBatchHelper(reply_msg, client);
}