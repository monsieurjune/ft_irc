/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircPRIVMSG.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scharuka <scharuka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 18:36:40 by scharuka          #+#    #+#             */
/*   Updated: 2025/02/01 19:27:07 by scharuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"

#include <iostream>

FtIrc::t_replyBatch FtIrc::ircPRIVMSG(FtIrc * const obj, Message const & msg, Client * const client)
{
	t_replyBatch replies;
	t_reply	reply;
	Message	reply_msg;

	if (msg.getParams().size() < 1)
	{
		reply_msg.setCommand(ERR_NEEDMOREPARAMS);
		reply_msg.pushParam(client->getNickname());
		reply_msg.pushParam("PRIVMSG");
		reply_msg.pushParam("Not enough parameters");
		reply.first = client;
		reply.second.push(reply_msg);
		replies.push_back(reply);
		return replies;
	}

	std::string target = msg.getParams().at(0);
	std::string message = msg.getParams().at(1);

	if (target[0] == '#') {
		Channel *channel = obj->getChannelByName(target);
		if (!channel) {
			reply_msg.setCommand(ERR_NOSUCHCHANNEL);
			reply_msg.pushParam(client->getNickname());
			reply_msg.pushParam(target);
			reply_msg.pushParam("No such channel");
			reply.first = client;
			reply.second.push(reply_msg);
			replies.push_back(reply);
			return replies;
		}
		if (!channel->hasThisClient(client)) {
			reply_msg.setCommand(ERR_CANNOTSENDTOCHAN);
			reply_msg.pushParam(client->getNickname());
			reply_msg.pushParam(target);
			reply_msg.pushParam("Cannot send to channel");
			reply.first = client;
			reply.second.push(reply_msg);
			replies.push_back(reply);
			return replies;
		}
		// Might need to boardcast the message here?
		Channel::t_userMap userMap = channel->getUserMap();
	}
	else
	{
		// Client *targetClient = obj->getClient(target);
		Client *targetClient = obj->getClientByNickname(target);
		if (!targetClient) {
			reply_msg.setCommand(ERR_NOSUCHNICK);
			reply_msg.pushParam(client->getNickname());
			reply_msg.pushParam(target);
			reply_msg.pushParam("No such nick/channel");
			reply.first = client;
			reply.second.push(reply_msg);
			replies.push_back(reply);
			return replies;
		}
		// Do I need to send the message out
	}

	return replies;
}