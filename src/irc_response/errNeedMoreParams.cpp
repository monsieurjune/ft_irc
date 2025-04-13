/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errNeedMoreParams.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 05:44:56 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/13 21:59:29 by tponutha         ###   ########.fr       */
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
	if (client->getNickname().empty())
	{
		reply_msg.pushParam("*");
	}
	else
	{
		reply_msg.pushParam(client->getNickname());
	}
	reply_msg.pushParam(message.getCommand());
	reply_msg.pushParam("Not enough parameters");

	// Put it in proper struct
	std::queue<Message>	queue;

	queue.push(reply_msg);

	FtIrc::t_reply		reply(client, queue);
	FtIrc::t_replyBatch	batch(1, reply);

	return batch;
}