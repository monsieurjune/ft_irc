/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircINVITE.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 18:33:17 by scharuka          #+#    #+#             */
/*   Updated: 2025/05/11 19:02:25 by tnualman         ###   ########.fr       */
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
	std::string					nick = sender->getNickname();

	reply_sender.first = sender;

    if (params.size() == 0)
    {
		reply_msg.setSource(obj->_serverName);
		reply_msg.setCommand(RPL_INVITELIST);
		// Iterate through list of channels in server
		std::map<std::string, Channel *>			tmpChannelMap = obj->_channelMapByName;
		std::map<std::string, Channel *>::iterator	it = tmpChannelMap.begin();
		for (it; it != tmpChannelMap.end(); it++)
		{
			Channel * channel = it->second;
			if (channel->isClientInvited(sender))
			{
				reply_msg.pushParam(nick);
				reply_msg.pushParam(channel->getName());
				reply_sender.second.push(reply_msg);
				reply_msg.resetParams();
			}
		}
		// RPL_ENDOFINVITELIST
		reply_msg.setCommand(RPL_INVITELIST);
		reply_msg.pushParam(nick);
		reply_msg.pushParam("End of /INVITE list.");
		reply_sender.second.push(reply_msg);
		batch.push_back(reply_sender);
		return (batch);
    }
    
    if (params.size() == 1 || params.at(0).empty() || params.at(1).empty())
	{
		return (obj->errNeedMoreParams(sender, message));
	}

    std::string invitee_nickname = params.at(0);
    Client *    invitee = obj->getClientByNickname(invitee_nickname);
    std::string channel_name = params.at(1);
    Channel *   channel = obj->getChannelByName(channel_name);

    // ERR_NOSUCHCHANNEL or which error response?
    if (!channel)
    {
        reply_msg.setSource(obj->_serverName);
		reply_msg.setCommand(ERR_NOSUCHCHANNEL);
		reply_msg.pushParam(sender->getNickname());
		reply_msg.pushParam(channel_name.empty() ? "*" : channel_name);
		reply_msg.pushParam("No such channel.");
		reply_sender.second.push(reply_msg);
		reply_msg.resetParams();
		batch.push_back(reply_sender);
		return (batch);
    }

    // ERR_NOSUCHNICK (not described in the modern doc, but I think this is needed.)
    if (!invitee)
    {
        // reply_msg.setSource(obj->_serverName);
		// reply_msg.setCommand(ERR_NOSUCHCHANNEL);
		// reply_msg.pushParam(sender->getNickname());
		// reply_msg.pushParam(channel_name.empty() ? "*" : channel_name);
		// reply_msg.pushParam("No such channel.");
		// reply_sender.second.push(reply_msg);
		// reply_msg.resetParams();
		// batch.push_back(reply_sender);
		// return (batch);
    }

	// // Primary for loop here.
	// for (unsigned long idx = 0; idx < channel_name_vec.size(); idx++)
	// {
	// 	std::string	channel_name = channel_name_vec.at(idx);
	// 	Channel *	channel = channel_name.empty() ? NULL : obj->getChannelByName(channel_name);

	// 	// ERR_NOSUCHCHANNEL has different meaning!

	// 	// Validate channel_name first.
	// 	if (channel_name.at(0) != '#' || !channel)
	// 	{
	// 		reply_msg.setSource(obj->_serverName);
	// 		reply_msg.setCommand(ERR_NOSUCHCHANNEL);
	// 		reply_msg.pushParam(sender->getNickname());
	// 		reply_msg.pushParam(channel_name.empty() ? "*" : channel_name);
	// 		reply_msg.pushParam("No such channel.");
	// 		reply_sender.second.push(reply_msg);
	// 		reply_msg.resetParams();
	// 		batch.push_back(reply_sender);
	// 		continue ;
	// 	}

	// 	if (!(channel->hasThisClient(sender)))
	// 	{
	// 		reply_msg.setSource(obj->_serverName);
	// 		reply_msg.setCommand(ERR_NOTONCHANNEL);
	// 		reply_msg.pushParam(sender->getNickname());
	// 		reply_msg.pushParam(channel_name.empty() ? "*" : channel_name);
	// 		reply_msg.pushParam("You're not on that channel.");
	// 		reply_sender.second.push(reply_msg);
	// 		reply_msg.resetParams();
	// 		batch.push_back(reply_sender);
	// 		continue ;
	// 	}

    //     // General/valid PARTing case here.
	// 	{
	// 		channel->deleteUserFromChannel(sender);
	// 		reply_msg.setSource(sender->constructSource());
	// 		reply_msg.setCommand("PART");
	// 		reply_msg.pushParam(channel_name);
	// 		if (!reason.empty())
    //         {
    //             reply_msg.pushParam(reason);
    //         }
    //         reply_sender.second.push(reply_msg);
	// 		batch.push_back(reply_sender);
	// 		// PARTing announcement to other channel members here.	
	// 		obj->pushChannelReplyOthers(reply_msg, channel, batch, sender);
	// 		reply_msg.resetParams();
	// 		continue ;
	// 	}
	// }
	return (batch);
}