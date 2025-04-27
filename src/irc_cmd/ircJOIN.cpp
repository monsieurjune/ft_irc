/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircJOIN.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 19:11:58 by tnualman          #+#    #+#             */
/*   Updated: 2025/04/27 16:47:35 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "std/ft_cppstd.hpp"

FtIrc::t_replyBatch FtIrc::ircJOIN(FtIrc * const obj, Message const & message, Client * const sender)
{
	std::vector<std::string>	params = message.getParams();
	Message						reply_msg;
	t_reply						reply_sender;
	t_replyBatch				batch;

	if (params.size() < 1 || params.at(0).empty())
	{
		return (obj->errNeedMoreParams(sender, message));
	}
	
	// Special case: leaving all channels. (WIP)
	// WIP WIP WIP
	if (params.at(0) == "0")
	{
		std::set<Channel*>	channelSet = obj->getChannelSetByClient(sender);
		for (std::set<Channel*>::iterator it = channelSet.begin(); it != channelSet.end(); it++)
		{
			// reply_msg is reuesd here to push a message into ircPART().
			reply_msg.setSource(obj->_serverName);
			reply_msg.setCommand("PART");
			reply_msg.pushParam((*it)->getName());
			// Call PART for each channel here.
			// (pseudo) batch.concat(obj->ircPART(obj, reply_msg, sender));
		}
		return (batch);
	}
	// WIP WIP WIP

	reply_sender.first = sender;

	std::vector<std::string>	channel_name_vec = ft_std::split(params.at(0), ",");
	std::vector<std::string>	channel_key_vec;
	size_t						idx;
	size_t						idx_temp;

	// Assigning channel_key_vec.
	// and matching numbers of chennel and keys in vectors by adding empty strings to channel missing keys.
	if (params.size() < 2 || params.at(1).empty())
	{
		idx_temp = 0;
	}
	else
	{
		channel_key_vec = ft_std::split(params.at(1), ",");
		idx_temp = channel_key_vec.size();
	}

	for (idx = idx_temp; idx < channel_name_vec.size(); idx++)
	{
		channel_key_vec.push_back("");
	}

	// Primary for loop here.
	for (idx = 0; idx < channel_name_vec.size(); idx++)
	{
		std::string	channel_name = channel_name_vec.at(idx);
		Channel *	channel = channel_name.empty() ? NULL : obj->getChannelByName(channel_name);

		// ERR_NOSUCHCHANNEL has different meaning!

		// Validate channel_name first.
		if (channel_name.at(0) != '#')
		{
			reply_msg.setSource(obj->_serverName);
			reply_msg.setCommand(ERR_NOSUCHCHANNEL);
			reply_msg.pushParam(sender->getNickname());
			reply_msg.pushParam(channel_name.empty() ? "*" : channel_name);
			reply_msg.pushParam("No such channel.");
			reply_sender.second.push(reply_msg);
			continue ;
		}
		
		// Create new channel if channel does not already exist.
		if (!channel)
		{
			obj->createChannel(channel_name, sender);
			// Reply message(s) to sender for newly created channel here.
			channel = obj->getChannelByName(channel_name);
			std::string sender_source = (sender->getUsername().empty() || sender->getHost().empty())
				? sender->getNickname() : sender->constructSource();
			reply_msg.setSource(":" + sender_source);
			reply_msg.setCommand("JOIN");
			reply_msg.pushParam(channel_name);
			reply_sender.second.push(reply_msg);
			// (WIP) NameReply: how to?
			// (pseudocode) batch.concat(obj->rplNameReply(sender, channel));
			continue ; 
		}

		if (channel->hasThisClient(sender))
		{
			continue ;
		}

		if (channel->hasThisMode(MODE_USERLIMIT) && channel->getUserCount() >= channel->getUserCountLimit())
		{
			reply_msg.setSource(obj->_serverName);
			reply_msg.setCommand(ERR_CHANNELISFULL);
			reply_msg.pushParam(sender->getNickname());
			reply_msg.pushParam(channel_name);
			reply_msg.pushParam("Cannot join channel (+l).");
			reply_sender.second.push(reply_msg);
			continue ;
		}

		if (channel->hasThisMode(MODE_INVITEONLY) && !(channel->isClientInvited(sender)))
		{
			reply_msg.setSource(obj->_serverName);
			reply_msg.setCommand(ERR_INVITEONLYCHAN);
			reply_msg.pushParam(sender->getNickname());
			reply_msg.pushParam(channel_name);
			reply_msg.pushParam("Cannot join channel (+i).");
			reply_sender.second.push(reply_msg);
			continue ;
		}

		if (channel->hasThisMode(MODE_CHANNELKEY) && channel_key_vec.at(idx) != channel->getPassword())
		{
			reply_msg.setSource(obj->_serverName);
			reply_msg.setCommand(ERR_BADCHANNELKEY);
			reply_msg.pushParam(sender->getNickname());
			reply_msg.pushParam(channel_name);
			reply_msg.pushParam("Cannot join channel (+k).");
			reply_sender.second.push(reply_msg);
			continue ;
		}

		// General/valid joining case here.
		{
			channel->addUserToChannel(sender, "");
			// (WIP) Replies to sender for the existing channel here.
			std::string sender_source = (sender->getUsername().empty() || sender->getHost().empty())
				? sender->getNickname() : sender->constructSource();
			reply_msg.setSource(":" + sender_source);
			reply_msg.setCommand("JOIN");
			reply_msg.pushParam(channel_name);
			reply_sender.second.push(reply_msg);
			if (!channel->getTopic().empty())
			{
				// RPL_TOPIC
				// RPL_TOPICWHOTIME
			}
			// (WIP) NameReply: how to?
			// (pseudocode) batch.concat(obj->rplNameReply(sender, obj->getChannelByName(channel_name)));
			
			// TODO: Then, code for announcement to channel members here.
		}
	}
	batch.push_back(reply_sender);

	return (batch);
}
