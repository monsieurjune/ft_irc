/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rplNameReply.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:34:14 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/20 12:22:59 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "ft_irc/FtIrcFunctionHelper.hpp"

// "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}"

static Message   sb_rpl_NAMREPLY(FtIrc * const obj, Client * const sender, Channel * const channel, Client * const client)
{
    Message reply_msg;

    // Creating MSG
    reply_msg.setSource(obj->getServerName());
    reply_msg.setCommand(RPL_NAMREPLY);
    reply_msg.pushParam(sender->getNickname().empty() ? "*" : sender->getNickname());
    reply_msg.pushParam("="); // there is only public channel here
    reply_msg.pushParam(channel->getName());

    // Construct Trailing Params
    if (channel->hasThisClientMembershipMode(client, MODE_OPERATOR))
    {
        reply_msg.pushParam("@" + client->getNickname());
    }
    else
    {
        reply_msg.pushParam(client->getNickname());
    }

    return reply_msg;
}

// "<client> <channel> :End of /NAMES list"

static Message  sb_rpl_ENDOFNAMES(FtIrc * const obj, Client * const sender, Channel * const channel)
{
    Message reply_msg;

    // Creating MSG
    reply_msg.setSource(obj->getServerName());
    reply_msg.setCommand(RPL_ENDOFNAMES);
    reply_msg.pushParam(sender->getNickname());
    reply_msg.pushParam(channel->getName());
    reply_msg.pushParam("End of /NAMES list");

    return reply_msg;
}

FtIrc::t_replyBatch FtIrc::rplNameReply(Client * const client, Channel * const channel)
{
    std::set<Client*>           clientSet = getClientSetByChannel(channel);
    std::set<Client*>::iterator it;
    t_reply                     reply;

    // Set Sender
    reply.first = client;

    // Loop to create Name List
    for (it = clientSet.begin(); it != clientSet.end(); it++)
    {
        Message msg = sb_rpl_NAMREPLY(this, client, channel, *it);

        reply.second.push(msg);
    }

    // Put ENDOFNAMES in msg
    reply.second.push(sb_rpl_ENDOFNAMES(this, client, channel));

    return t_replyBatch(1, reply);
}