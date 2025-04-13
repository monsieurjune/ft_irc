/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrcHelper.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 03:08:52 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/14 04:01:20 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
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

void    FtIrc::nicknameMessageHelper(Message & reply_msg, Client * const client)
{
    if (client->getNickname().empty())
    {
        reply_msg.pushParam(IRC_EMPTY_STRING);
    }
    else
    {
        reply_msg.pushParam(client->getNickname());
    }
}

FtIrc::t_replyBatch FtIrc::singleReplyBatchHelper(Message const & reply_msg, Client * const client)
{
    std::queue<Message> queue;

    queue.push(reply_msg);

    FtIrc::t_reply		reply(client, queue);
	FtIrc::t_replyBatch	batch(1, reply);

	return batch;
}