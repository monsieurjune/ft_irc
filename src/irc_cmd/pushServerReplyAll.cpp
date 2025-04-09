/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pushServerReplyAll.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:30:11 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/09 16:31:11 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "std/ft_cppstd.hpp"

void FtIrc::pushServerReplyAll(Message const & reply_msg, FtIrc::t_replyBatch & batch)
{
	std::map<int, Client*>::iterator	it_cmap = _clientMapByFd.begin();
	std::map<int, Client*>::iterator	it_cmap_end = _clientMapByFd.end();
	t_reply								reply;

	reply.second.push(reply_msg);

	for (it_cmap; it_cmap != it_cmap_end; it_cmap++)
	{
		reply.first = it_cmap->second;
		batch.push_back(reply);
	}
}
