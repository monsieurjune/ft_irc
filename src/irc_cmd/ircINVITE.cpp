/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircINVITE.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 18:33:17 by scharuka          #+#    #+#             */
/*   Updated: 2025/05/14 11:26:48 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "std/ft_cppstd.hpp"

FtIrc::t_replyBatch FtIrc::ircINVITE(FtIrc * const obj, Message const & message, Client * const sender)
{
    std::vector<std::string>	params = message.getParams();
	Message						reply_msg;
	t_reply						reply_sender;
	t_replyBatch				batch;
	std::string					sender_nick = sender->getNickname();

	reply_sender.first = sender;
	reply_msg.setSource(obj->_serverName);

    // INVITE_LIST
	if (params.size() == 0)
    {
		reply_msg.setCommand(RPL_INVITELIST);

		std::map<std::string, Channel *>			tmpChannelMap = obj->_channelMapByName;
		std::map<std::string, Channel *>::iterator	it;

		// Iterate through list of channels in serverc
		for (it = tmpChannelMap.begin(); it != tmpChannelMap.end(); it++)
		{
			Channel * channel = it->second;
			if (channel->isClientInvited(sender))
			{
				reply_msg.pushParam(sender_nick);
				reply_msg.pushParam(channel->getName());
				reply_sender.second.push(reply_msg);
				reply_msg.resetParams();
			}
		}
		// RPL_ENDOFINVITELIST
		reply_msg.setCommand(RPL_ENDOFINVITELIST);
		reply_msg.pushParam(sender_nick);
		reply_msg.pushParam("End of /INVITE list.");
		reply_sender.second.push(reply_msg);
		batch.push_back(reply_sender);
		return (batch);
    }

    // ERR_NEEDMOREPARAMS
	if (params.size() < 2 || params.at(0).empty() || params.at(1).empty())
	{
		return (obj->errNeedMoreParams(sender, message));
	}

    std::string invitee_nick = params.at(0);
    Client *    invitee = obj->getClientByNickname(invitee_nick);
    std::string channel_name = params.at(1);
    Channel *   channel = obj->getChannelByName(channel_name);

    // ERR_NOSUCHCHANNEL or which error response?
    if (channel_name.at(0) != '#' || !channel)
    {
		reply_msg.setCommand(ERR_NOSUCHCHANNEL);
		reply_msg.pushParam(sender_nick);
		reply_msg.pushParam(channel_name);
		reply_msg.pushParam("No such channel.");
		reply_sender.second.push(reply_msg);
		batch.push_back(reply_sender);
		return (batch);
    }

    // ERR_NOSUCHNICK (not described in the modern doc, but I think this is needed.)
    if (!invitee)
    {
		reply_msg.setCommand(ERR_NOSUCHNICK);
		reply_msg.pushParam(sender_nick);
		reply_msg.pushParam(invitee_nick);
		reply_msg.pushParam("No such nickname.");
		reply_sender.second.push(reply_msg);
		batch.push_back(reply_sender);
		return (batch);
    }

	// ERR_NOTONCHANNEL
	if (!(channel->hasThisClient(sender)))
	{
		reply_msg.setCommand(ERR_NOTONCHANNEL);
		reply_msg.pushParam(sender_nick);
		reply_msg.pushParam(channel_name);
		reply_msg.pushParam("You're not on that channel.");
		reply_sender.second.push(reply_msg);
		batch.push_back(reply_sender);
		return (batch);
	}

	// ERR_CHANOPRIVSNEEDED
	if (channel->hasThisMode(MODE_INVITEONLY) && !(channel->hasThisClientMembershipMode(sender, MODE_OPERATOR)))
	{
		reply_msg.setCommand(ERR_CHANOPRIVSNEEDED);
		reply_msg.pushParam(sender_nick);
		reply_msg.pushParam(channel_name);
		reply_msg.pushParam("You need channel operator privilege to send an invite for an invite-only channel.");
		reply_sender.second.push(reply_msg);
		batch.push_back(reply_sender);
		return (batch);
	}

	// ERR_USERONCHANNEL
	if (channel->hasThisClient(invitee))
	{
		reply_msg.setCommand(ERR_USERONCHANNEL);
		reply_msg.pushParam(sender_nick);
		reply_msg.pushParam(invitee_nick);
		reply_msg.pushParam(channel_name);
		reply_msg.pushParam("Invitee is already on channel/.");
		reply_sender.second.push(reply_msg);
		batch.push_back(reply_sender);
		return (batch);
	}

	// SUCCESS_CASE
	{
		reply_msg.setCommand(RPL_INVITING);
		reply_msg.pushParam(sender_nick);
		reply_msg.pushParam(invitee_nick);
		reply_msg.pushParam(channel_name);
		reply_sender.second.push(reply_msg);
		batch.push_back(reply_sender);
		reply_msg.resetParams();

		t_reply	reply_invitee;

		reply_invitee.first = invitee;
		reply_msg.setSource(sender->constructSource());
		reply_msg.setCommand("INVITE");
		reply_msg.pushParam(invitee_nick);
		reply_msg.pushParam(channel_name);
		reply_sender.second.push(reply_msg);
		batch.push_back(reply_invitee);

		return (batch);
	}
}
