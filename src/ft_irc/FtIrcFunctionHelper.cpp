/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrcFunctionHelper.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 00:40:27 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/20 12:23:53 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc/FtIrcFunctionHelper.hpp"

FtIrc::t_replyBatch singleReplySingleClientBatch(Message const & reply_msg, Client * const client)
{
    std::queue<Message> queue;

    queue.push(reply_msg);

    FtIrc::t_reply		reply(client, queue);
	FtIrc::t_replyBatch	batch(1, reply);

	return batch;
}

FtIrc::t_replyBatch singleReplyMultiClientBatch(Message const & reply_msg, std::set<Client*> const & clientSet)
{
    std::queue<Message>                 queue;
    std::set<Client*>::const_iterator   it;
	FtIrc::t_replyBatch	                batch(clientSet.size());

    queue.push(reply_msg);

    for (it = clientSet.begin(); it != clientSet.end(); it++)
    {
        FtIrc::t_reply  reply(*it, queue);

        batch.push_back(reply);
    }

    return batch;
}