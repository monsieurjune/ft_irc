/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errUnknownCmd.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 05:45:02 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/15 02:15:28 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "ft_irc/FtIrcFunctionHelper.hpp"

FtIrc::t_replyBatch	FtIrc::errUnknownCmd(Client * const client, std::string const& cmd)
{
	Message	reply_msg;

	// Creating MSG
	reply_msg.setSource(_serverName);
	reply_msg.setCommand(ERR_UNKNOWNCOMMAND);
	nicknameMessageHelper(reply_msg, client);
	reply_msg.pushParam(cmd);
	reply_msg.pushParam("Unknown command");

	return singleReplySingleClientBatch(reply_msg, client);
}