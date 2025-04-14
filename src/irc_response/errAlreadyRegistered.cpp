/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errAlreadyRegistered.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 05:44:52 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/15 02:15:28 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "ft_irc/FtIrcFunctionHelper.hpp"

FtIrc::t_replyBatch	FtIrc::errAlreadyRegistered(Client * const client)
{
	Message	reply_msg;

	// Creating MSG
	reply_msg.setSource(_serverName);
	reply_msg.setCommand(ERR_ALREADYREGISTERED);
	nicknameMessageHelper(reply_msg, client);
	reply_msg.pushParam("You may not reregister");

	return singleReplySingleClientBatch(reply_msg, client);
}