/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rplWelcome.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:33:35 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/18 06:47:59 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "ft_irc/FtIrcFunctionHelper.hpp"
#include "std/ft_cppstd.hpp"

// "<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]"

static Message  sb_rpl_WELCOME(FtIrc * const obj, Client * const client)
{
    Message     reply_msg;
    std::string trailing;

    trailing = "Welcome to the " + obj->getNetworkName() + " Network, ";
    trailing += client->constructSource();

    reply_msg.setSource(obj->getServerName());
    reply_msg.setCommand(RPL_WELCOME);
    reply_msg.pushParam(client->getNickname().empty() ? "*" : client->getNickname());
    reply_msg.pushParam(trailing);

    return reply_msg;
}

// "<client> :Your host is <servername>, running version <version>"

static Message  sb_rpl_YOURHOST(FtIrc * const obj, Client * const client)
{
    Message     reply_msg;
    std::string trailing;

    trailing = "Your host is " + obj->getServerName() + ", running version " + FT_IRC_VERSION;

    reply_msg.setSource(obj->getServerName());
    reply_msg.setCommand(RPL_YOURHOST);
    reply_msg.pushParam(client->getNickname().empty() ? "*" : client->getNickname());
    reply_msg.pushParam(trailing);

    return reply_msg;
}

// "<client> :This server was created <datetime>"

static Message  sb_rpl_CREATED(FtIrc * const obj, Client * const client)
{
    char                timebuff[128];
    Message             reply_msg;
    std::string         trailing;
    std::time_t const&  start       = obj->getTimeServerCreated();
    struct std::tm*     timeinfo    = std::localtime(&start);

    std::strftime(timebuff, 128, "%H:%M:%S %d %B %Y", timeinfo);
    trailing = std::string("This server was created ") + std::string(timebuff);

    reply_msg.setSource(obj->getServerName());
    reply_msg.setCommand(RPL_CREATED);
    reply_msg.pushParam(client->getNickname().empty() ? "*" : client->getNickname());
    reply_msg.pushParam(trailing);

    return reply_msg;
}

// "<client> <servername> <version> <available user modes> <available channel modes> [<channel modes with a parameter>]"

static Message  sb_rpl_MYINFO(FtIrc * const obj, Client * const client)
{
    Message reply_msg;

    reply_msg.setSource(obj->getServerName());
    reply_msg.setCommand(RPL_MYINFO);
    reply_msg.pushParam(client->getNickname().empty() ? "*" : client->getNickname());
    reply_msg.pushParam(obj->getServerName());
    reply_msg.pushParam(FT_IRC_VERSION);
    reply_msg.pushParam("o");
    reply_msg.pushParam("itkol");
    reply_msg.pushParam("kol");

    return reply_msg;
}

// "<client> <1-13 tokens> :are supported by this server"

static Message sb_rpl_ISUPPORT(FtIrc * const obj, Client * const client)
{
    Message reply_msg;

    reply_msg.setSource(obj->getServerName());
    reply_msg.setCommand(RPL_ISUPPORT);
    reply_msg.pushParam(client->getNickname().empty() ? "*" : client->getNickname());
    reply_msg.pushParam("CHANTYPES=#");
    reply_msg.pushParam("PREFIX=(o)@");
    reply_msg.pushParam("MODES=" + ft_std::itoa(3));
    reply_msg.pushParam("NICKLEN=" + ft_std::itoa(NICKLEN));
    reply_msg.pushParam("TOPICLEN=" + ft_std::itoa(TOPICLEN));
    reply_msg.pushParam("CHANNELLEN=" + ft_std::itoa(CHANNELLEN));
    reply_msg.pushParam("USERLEN=" + ft_std::itoa(USERLEN));
    reply_msg.pushParam("NETWORK=" + obj->getNetworkName());
    reply_msg.pushParam("are supported by this server");

    return reply_msg;
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
    issuport_reply.first = client;
    issuport_reply.second.push(sb_rpl_ISUPPORT(this, client));

    // 375, 372, 376

    // Append to Reply Batch
    FtIrc::t_replyBatch batch(2);

    batch.push_back(welcome_reply);
    batch.push_back(issuport_reply);

    return batch;
}