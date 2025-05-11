/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircJOIN.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 19:11:58 by tnualman          #+#    #+#             */
/*   Updated: 2025/05/11 18:51:50 by tnualman         ###   ########.fr       */
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
	
	// Special case: leaving all channels. (Needs testing)
	if (params.at(0) == "0")
	{
		std::set<Channel*>	channelSet = obj->getChannelSetByClient(sender);
		for (std::set<Channel*>::iterator it = channelSet.begin(); it != channelSet.end(); it++)
		{
			// reply_msg is reuesd here to push a message into ircPART().
			reply_msg.setSource("");
			reply_msg.setCommand("PART");
			reply_msg.pushParam((*it)->getName());
			// Call PART for each channel here.
			// (pseudocode) batch.concat(obj->ircPART(obj, reply_msg, sender));
			t_replyBatch tmp = obj->ircPART(obj, reply_msg, sender);
			batch.insert(batch.end(), tmp.begin(), tmp.end());
		}
		return (batch);
	}

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
			reply_msg.resetParams();
			batch.push_back(reply_sender);
			queueclear(reply_sender.second);
			continue ;
		}
		
		// Create new channel if channel does not already exist.
		if (!channel)
		{
			obj->createChannel(channel_name, sender);
			// Reply message(s) to sender for newly created channel starts from here.
			channel = obj->getChannelByName(channel_name);
			reply_msg.setSource(sender->constructSource());
			reply_msg.setCommand("JOIN");
			reply_msg.pushParam(channel_name);
			reply_sender.second.push(reply_msg);
			reply_msg.resetParams();
			// RPL_NAMREPLY
			// Getting messages from inside the reply batch.
			std::queue<Message> tmp = obj->rplNameReply(sender, channel).at(0).second;
			while (!tmp.empty())
			{
				reply_sender.second.push(tmp.front());
				tmp.pop();
			}
			batch.push_back(reply_sender);
			queueclear(reply_sender.second);
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
			reply_msg.resetParams();
			batch.push_back(reply_sender);
			queueclear(reply_sender.second);
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
			reply_msg.resetParams();
			batch.push_back(reply_sender);
			queueclear(reply_sender.second);
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
			reply_msg.resetParams();
			batch.push_back(reply_sender);
			queueclear(reply_sender.second);
			continue ;
		}

		// General/valid joining case here.
		{
			channel->addUserToChannel(sender, "");
			// Replies to sender for the existing channel here.
			reply_msg.setSource(sender->constructSource());
			reply_msg.setCommand("JOIN");
			reply_msg.pushParam(channel_name);
			reply_sender.second.push(reply_msg);
			reply_msg.resetParams();
			if (!channel->getTopic().empty())
			{
				// RPL_TOPIC
				reply_msg.setSource(obj->_serverName);
				reply_msg.setCommand(RPL_TOPIC);
				reply_msg.pushParam(sender->getNickname());
				reply_msg.pushParam(channel->getName());
				reply_msg.pushParam(channel->getTopic());
				reply_sender.second.push(reply_msg);
				reply_msg.resetParams();
				// RPL_TOPICWHOTIME
				reply_msg.setSource(obj->_serverName);
				reply_msg.setCommand(RPL_TOPICWHOTIME);
				reply_msg.pushParam(sender->getNickname());
				reply_msg.pushParam(channel->getName());
				reply_msg.pushParam(channel->getTopicSetter());
				reply_msg.pushParam(ft_std::itoa(channel->getTimeTopicSet()));
				reply_sender.second.push(reply_msg);
				reply_msg.resetParams();
			}
			// RPL_NAMREPLY
			// Getting messages from inside the reply batch.
			std::queue<Message> tmp = obj->rplNameReply(sender, channel).at(0).second;
			while (!tmp.empty())
			{
				reply_sender.second.push(tmp.front());
				tmp.pop();
			}
			batch.push_back(reply_sender);
			queueclear(reply_sender.second);
			
			// Joining announcement to channel members here.	
			reply_msg.setSource(sender->constructSource());
			reply_msg.setCommand("JOIN");
			reply_msg.pushParam(channel->getName());
			obj->pushChannelReplyOthers(reply_msg, channel, batch, sender);
			reply_msg.resetParams();
			
			continue ;
		}
	}

	return (batch);
}
