/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rplWelcome.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:33:35 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/19 00:49:39 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "ft_irc/FtIrcFunctionHelper.hpp"

static Message  sb_rpl_WELCOME(FtIrc * const obj, Client * const client)
{
    
}

static Message  sb_rpl_YOURHOST(FtIrc * const obj, Client * const client)
{
    
}

static Message  sb_rpl_CREATED(FtIrc * const obj, Client * const client)
{
    
}

static Message  sb_rpl_MYINFO(FtIrc * const obj, Client * const client)
{

}

static FtIrc::t_reply   sb_rpl_ISUPPORT(FtIrc * const obj, Client * const client)
{

}

FtIrc::t_replyBatch FtIrc::rplWelcome(Client * const client)
{
    FtIrc::t_reply  welcome_reply;
    FtIrc::t_reply  issuport_reply;

    // 001, 002, 003, 004
    welcome_reply.first = client;
    welcome_reply.second.push(sb_rpl_WELCOME(this, client));
    welcome_reply.second.push(sb_rpl_YOURHOST(this, client));
    welcome_reply.second.push(sb_rpl_CREATED(this, client));
    welcome_reply.second.push(sb_rpl_MYINFO(this, client));

    // 005
    issuport_reply = sb_rpl_ISUPPORT(this, client);

    // 375, 372, 376
    // TODO: do later

    // Append to Reply Batch
    FtIrc::t_replyBatch batch(2);

    batch.push_back(welcome_reply);
    batch.push_back(issuport_reply);

    return batch;
}