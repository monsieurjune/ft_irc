/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircKICK.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 15:51:16 by tnualman          #+#    #+#             */
/*   Updated: 2025/04/13 23:15:36 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "std/ft_cppstd.hpp"

FtIrc::t_replyBatch FtIrc::ircKICK(FtIrc * const obj, Message const & message, Client * const sender)
{
	std::vector<std::string>    params = message.getParams();
	Message			            reply_msg;
	t_reply			            reply_sender;
	t_replyBatch	            batch;

    reply_sender.first = sender;
	
	if (params.size() < 2 || params.at(0).empty() || params.at(1).empty())
	{
		return (obj->errNeedMoreParams(sender, message));
	}

    std::string channel_name = params.at(0);
    Channel *   channel = obj->getChannelByName(channel_name);
    
    if (!channel)
    {
        reply_msg.setSource(obj->getServerName());
        reply_msg.setCommand(ERR_NOSUCHCHANNEL);
        reply_msg.pushParam(sender->getNickname());
        reply_msg.pushParam("#" + channel_name);
        reply_msg.pushParam("No such channel.");
        reply_sender.second.push(reply_msg);
        batch.push_back(reply_sender);
        return (batch);
    }

    if (!(channel->hasThisClientMembershipMode(sender, MODE_OPERATOR)))
    {
        reply_msg.setSource(obj->getServerName());
        reply_msg.setCommand(ERR_CHANOPRIVSNEEDED);
        reply_msg.pushParam(sender->getNickname());
        reply_msg.pushParam("#" + channel_name);
        reply_msg.pushParam("You're not a channel operator.");
        reply_sender.second.push(reply_msg);
        batch.push_back(reply_sender);
        return (batch);
    }

    std::string reason;

    if (params.size() < 3 || params.at(2).empty())
    {
        reason = "No specific reason given.";
    }
    else
    {
        reason = params.at(2);
    }
    
    std::vector<std::string>    target_name_vec = ft_std::split(params.at(1), ",");
    
    for (std::vector<std::string>::iterator it = target_name_vec.begin(); it != target_name_vec.end(); it++)
    {
        if (it->empty())
        {
            continue ;
        }
        
        Client * target = obj->getClientByNickname(*it);

        if (!target)
        {
            reply_msg.setSource(obj->getServerName());
            reply_msg.setCommand(ERR_NOSUCHNICK);
            reply_msg.pushParam(sender->getNickname());
            reply_msg.pushParam(*it);
            reply_msg.pushParam("No such nickname.");
            reply_sender.second.push(reply_msg);
            continue ;
        }

        if (!channel->hasThisClient(target))
        {
            reply_msg.setSource(obj->getServerName());
            reply_msg.setCommand(ERR_USERNOTINCHANNEL);
            reply_msg.pushParam(sender->getNickname());
            reply_msg.pushParam(*it);
            reply_msg.pushParam("#" + channel_name);
            reply_msg.pushParam("They're not on that channel.");
            reply_sender.second.push(reply_msg);
            continue ;
        }

        channel->deleteUserFromChannel(target);

        reply_msg.setSource(sender->constructSource());
        reply_msg.setCommand("KICK");
        reply_msg.pushParam("#" + channel_name);
        reply_msg.pushParam(*it);
        reply_msg.pushParam(reason);
        obj->pushChannelReplyAll(reply_msg, channel, batch);
    }

    batch.push_back(reply_sender);
    return (batch);
}
