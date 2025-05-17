/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrcMethodHelper.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 00:40:23 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/17 20:40:31 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "ft_irc/FtIrcFunctionHelper.hpp"
#include "std/ft_cstd.hpp"

void    FtIrc::pushServerReplyAll(Message const & reply_msg, t_replyBatch & batch)
{
	std::map<int, Client*>::iterator	it_cmap;
	std::map<int, Client*>::iterator	it_cmap_end = _clientMapByFd.end();
	t_reply								reply;

	reply.second.push(reply_msg);

	for (it_cmap = _clientMapByFd.begin(); it_cmap != it_cmap_end; it_cmap++)
	{
		reply.first = it_cmap->second;
		batch.push_back(reply);
	}
}

void    FtIrc::pushChannelReplyAll(Message const & reply_msg, Channel * const channel, t_replyBatch & batch)
{
	Channel::t_userMap				userMap = channel->getUserMap();
	Channel::t_userMap::iterator	it_umap;
	Channel::t_userMap::iterator	it_umap_end = userMap.end();
	t_reply							reply;

	reply.second.push(reply_msg);

	for (it_umap = userMap.begin(); it_umap != it_umap_end; it_umap++)
	{
		reply.first = it_umap->first;
		batch.push_back(reply);
	}
}

void    FtIrc::pushChannelReplyOthers(Message const & reply_msg, Channel * const channel, t_replyBatch & batch, Client * sender)
{
	Channel::t_userMap				userMap = channel->getUserMap();
	Channel::t_userMap::iterator	it_umap;
	Channel::t_userMap::iterator	it_umap_end = userMap.end();
	t_reply							reply;

	reply.second.push(reply_msg);

	for (it_umap = userMap.begin(); it_umap != it_umap_end; it_umap++)
	{
		if (it_umap->first == sender)
		{
			continue ;
		}
		reply.first = it_umap->first;
		batch.push_back(reply);
	}
}

void	FtIrc::notifyErrTooLongOnThisClient(int const fd)
{
	Client*	ptr = getClientByFd(fd);

	if (ptr == NULL)
	{
		return;
	}

	// Apply ERR_TOOLONG to client
	t_replyBatch	batch = errInputTooLong(ptr);

	applyReplyBatchToClient(batch);
}

void	FtIrc::notifyQuitOnThisClient(int const fd, std::string const& quit_msg)
{
	Client*	ptr = getClientByFd(fd);

	if (ptr == NULL)
	{
		return;
	}

	// Get Client Set that joining same channels as this client
	std::set<Channel*>	channelSet = getChannelSetByClient(ptr);
	std::set<Client*>	clientSet = getClientSetByChannelSet(channelSet);
	Message				msg;
	std::string			trailing = std::string("QUIT: ") + quit_msg;

	// Set QUIT Message
	msg.setSource(ptr->constructSource());
	msg.setCommand("QUIT");
	msg.pushParam(trailing);
	
	// Get Batch & Send to everyone
	t_replyBatch ret_batch = singleReplyMultiClientBatch(msg, clientSet);

	applyReplyBatchToClient(ret_batch);

	// Remove this client
	deleteClient(fd);
}