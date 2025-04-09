/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pushChannelReplyAll.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:26:41 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/09 16:27:02 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "std/ft_cppstd.hpp"

void FtIrc::pushChannelReplyAll(Message const & reply_msg, Channel * const channel, FtIrc::t_replyBatch & batch)
{
	Channel::t_userMap				userMap = channel->getUserMap();
	Channel::t_userMap::iterator	it_umap = userMap.begin();
	Channel::t_userMap::iterator	it_umap_end = userMap.end();
	t_reply							reply;

	reply.second.push(reply_msg);

	for (it_umap; it_umap != it_umap_end; it_umap++)
	{
		reply.first = it_umap->first;
		batch.push_back(reply);
	}
}
