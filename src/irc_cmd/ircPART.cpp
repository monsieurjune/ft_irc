/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircPART.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 18:33:17 by scharuka          #+#    #+#             */
/*   Updated: 2025/05/18 07:51:14 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "std/ft_cppstd.hpp"

static void	queueclear(std::queue<Message> &q)
{
	std::queue<Message>	empty;
	std::swap(q, empty);
}

FtIrc::t_replyBatch	FtIrc::ircPART(FtIrc * const obj, Message const & message, Client * const sender)
{
	std::vector<std::string>	params = message.getParams();
	Message						reply_msg;
	t_reply						reply_sender;
	t_replyBatch				batch;

	if (params.size() < 1 || params.at(0).empty())
	{
		return (obj->errNeedMoreParams(sender, message));
	}

    reply_sender.first = sender;
    std::string reason = (params.size() < 2 || params.at(1).empty()) ? "" : params.at(1);
    std::vector<std::string>	channel_name_vec = ft_std::split(params.at(0), ",");

	// Primary for loop here.
	for (unsigned long idx = 0; idx < channel_name_vec.size(); idx++)
	{
		std::string	channel_name = channel_name_vec.at(idx);
		Channel *	channel = channel_name.empty() ? NULL : obj->getChannelByName(channel_name);

		// ERR_NOSUCHCHANNEL has different meaning!

		// Validate channel_name first.
		if (channel_name.at(0) != '#' || !channel)
		{
			reply_msg.setSource(obj->_serverName);
			reply_msg.setCommand(ERR_NOSUCHCHANNEL);
			reply_msg.pushParam(sender->getNickname());
			reply_msg.pushParam(channel_name.empty() ? "*" : channel_name);
			reply_msg.pushParam("No such channel.");
			reply_sender.second.push(reply_msg);
			reply_msg.resetParams();
			batch.push_back(reply_sender);
			queueclear(reply_sender.second);
			continue ;
		}

		if (!(channel->hasThisClient(sender)))
		{
			reply_msg.setSource(obj->_serverName);
			reply_msg.setCommand(ERR_NOTONCHANNEL);
			reply_msg.pushParam(sender->getNickname());
			reply_msg.pushParam(channel_name.empty() ? "*" : channel_name);
			reply_msg.pushParam("You're not on that channel.");
			reply_sender.second.push(reply_msg);
			reply_msg.resetParams();
			batch.push_back(reply_sender);
			queueclear(reply_sender.second);
			continue ;
		}

        // General/valid PARTing case here.
		{
			obj->deleteClientFromChannel(channel_name, sender);
			// channel->deleteUserFromChannel(sender);
			reply_msg.setSource(sender->constructSource());
			reply_msg.setCommand("PART");
			reply_msg.pushParam(channel_name);
			if (!reason.empty())
            {
                reply_msg.pushParam(reason);
            }
            reply_sender.second.push(reply_msg);
			batch.push_back(reply_sender);
			queueclear(reply_sender.second);
			// PARTing announcement to other channel members here.	
			obj->pushChannelReplyOthers(reply_msg, channel, batch, sender);
			reply_msg.resetParams();
			continue ;
		}
	}
	return (batch);
}
