/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errNotRegistered.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 05:44:59 by tponutha          #+#    #+#             */
/*   Updated: 2025/02/01 16:38:09 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc/ircReply.hpp"

FtIrc::t_replyBatch	errNotRegistered(FtIrc * const obj, Client * const client)
{
	Message	reply_msg;

	// Creating MSG
	reply_msg.setSource(obj->getServerName());
	reply_msg.setCommand(ERR_NOTREGISTERED);
	if (client->getNickname().empty())
	{
		reply_msg.pushParam("*");
	}
	else
	{
		reply_msg.pushParam(client->getNickname());
	}
	reply_msg.pushParam("You have not registered");

	// Put it in proper struct
	std::queue<Message>	queue;

	queue.push(reply_msg);

	FtIrc::t_reply		reply(client, queue);
	FtIrc::t_replyBatch	batch(1, reply);

	return batch;
}