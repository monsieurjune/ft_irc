/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errInputTooLong.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 05:45:40 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/13 22:00:01 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "std/ft_cppstd.hpp"

FtIrc::t_replyBatch	FtIrc::errInputTooLong(Client * const client)
{
	Message	reply_msg;

	// Creating MSG
	reply_msg.setSource(_serverName);
	reply_msg.setCommand(ERR_INPUTTOOLONG);
	if (client->getNickname().empty())
	{
		reply_msg.pushParam("*");
	}
	else
	{
		reply_msg.pushParam(client->getNickname());
	}
	reply_msg.pushParam("Input line was too long");

	// Put it in proper struct
	std::queue<Message>	queue;

	queue.push(reply_msg);

	FtIrc::t_reply		reply(client, queue);
	FtIrc::t_replyBatch	batch(1, reply);

	return batch;
}