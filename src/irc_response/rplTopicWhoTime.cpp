/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rplTopicWhoTime.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:22:07 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/20 12:20:37 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "std/ft_cppstd.hpp"
#include "ft_irc/FtIrcFunctionHelper.hpp"

FtIrc::t_replyBatch FtIrc::rplTopicWhoTime(Client * const client, Channel * const channel)
{
	Message			reply_msg;
	t_reply			reply;
	t_replyBatch	batch;

	reply_msg.setSource(_serverName);
	reply_msg.setCommand(RPL_TOPIC);
	reply_msg.pushParam(client->getNickname().empty() ? "*" : client->getNickname());
	reply_msg.pushParam(channel->getName());
	reply_msg.pushParam(channel->getTopic());
	reply.first = client;
	reply.second.push(reply_msg);
	reply_msg.resetParams();
	reply_msg.setCommand(RPL_TOPICWHOTIME);
	reply_msg.pushParam(client->getNickname());
	reply_msg.pushParam(channel->getName());
	reply_msg.pushParam(channel->getTopicSetter());
	reply_msg.pushParam(ft_std::itoa(channel->getTimeTopicSet()));
	reply.second.push(reply_msg);
	batch.push_back(reply);

	return (batch);
}