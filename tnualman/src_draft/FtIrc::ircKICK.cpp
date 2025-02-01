/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc::ircKICK.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 15:51:16 by tnualman          #+#    #+#             */
/*   Updated: 2025/02/01 16:04:57 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FtIrc.hpp"

std::string	ltoa(long num)
{
	std::ostringstream oss;

	oss << num;
	return oss.str();
}

FtIrc::t_replyBatch FtIrc::ircTOPIC(FtIrc * const obj, Message const & message, Client * const sender)
{
	int				param_count = message.getParams().size();
	Message			reply_msg;
	t_reply			reply;
	t_replyBatch	batch;

	reply_msg.setSource(obj->getServerName());
	
	if (param_count < 1)
	{
		return (obj->err_NeedMoreParams(message, sender));
	}

	std::string	channel_name = message.getParams().at(0);
	Channel *	channel = obj->getChannelByName(channel_name);
	
	if (!channel)
	{;
		reply_msg.setCommand(ERR_NOSUCHCHANNEL);
		reply_msg.pushParam(sender->getNickname());
		reply_msg.pushParam("#" + channel_name);
		reply_msg.pushParam("No such channel");
		reply.first = sender;
		reply.second.push(reply_msg);
		batch.push_back(reply);
		return (batch);
	}

	if (param_count == 1)
	{
		if (channel->getTopic().empty())
		{
			reply_msg.setCommand(RPL_NOTOPIC);
			reply_msg.pushParam(sender->getNickname());
			reply_msg.pushParam("#" + channel_name);
			reply_msg.pushParam("No topic is set");
			reply.first = sender;
			reply.second.push(reply_msg);
			batch.push_back(reply);
			return (batch);
		}
		else
		{
			return (obj->rpl_Topic_WhoTime(message, sender, channel));
		}
	}

	if (!channel->hasThisClient(sender))
	{
		reply_msg.setCommand(ERR_NOTONCHANNEL);
		reply_msg.pushParam(sender->getNickname());
		reply_msg.pushParam("#" + channel_name);
		reply_msg.pushParam("You're not on that channel");
		reply.first = sender;
		reply.second.push(reply_msg);
		batch.push_back(reply);
		return (batch);
	}

	if (channel->hasThisMode(MODE_PROTECTTOPIC) && !channel->hasThisClientMembershipMode(sender, MODE_OPERATOR))
	{
		reply_msg.setCommand(ERR_CHANOPRIVSNEEDED);
		reply_msg.pushParam(sender->getNickname());
		reply_msg.pushParam("#" + channel_name);
		reply_msg.pushParam("You're not channel operator");
		reply.first = sender;
		reply.second.push(reply_msg);
		batch.push_back(reply);
		return (batch);
	}
	
	// Topic will be changed (general case) from here.

	channel->setTopic(message.getParams().at(1), sender);

	reply_msg.setCommand("TOPIC");
	reply_msg.pushParam("#" + channel_name);
	reply_msg.pushParam(channel->getTopic());
	reply.second.push(reply_msg);
	
	Channel::t_userMap userMap = channel->getUserMap();
	for (Channel::t_userMap::iterator it = userMap.begin(); it != userMap.end(); it++)
	{
		reply.first = it->first;
		batch.push_back(reply);
	}

	return (batch);
}
