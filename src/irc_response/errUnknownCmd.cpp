/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errUnknownCmd.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 05:45:02 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/13 21:55:56 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "std/ft_cppstd.hpp"

FtIrc::t_replyBatch	FtIrc::errUnknownCmd(Client * const client, std::string const& cmd)
{
	Message	reply_msg;

	// Creating MSG
	reply_msg.setSource(_serverName);
	reply_msg.setCommand(ERR_UNKNOWNCOMMAND);
	if (client->getNickname().empty())
	{
		reply_msg.pushParam("*");
	}
	else
	{
		reply_msg.pushParam(client->getNickname());
	}
	reply_msg.pushParam(cmd);
	reply_msg.pushParam("Unknown command");

	// Put it in proper struct
	std::queue<Message>	queue;

	queue.push(reply_msg);

	FtIrc::t_reply		reply(client, queue);
	FtIrc::t_replyBatch	batch(1, reply);

	return batch;
}