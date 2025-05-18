/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircPRIVMSG_new.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 18:36:40 by scharuka          #+#    #+#             */
/*   Updated: 2025/05/18 10:22:03 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "std/ft_cppstd.hpp"

FtIrc::t_replyBatch FtIrc::ircPRIVMSG(FtIrc * const obj, Message const & msg, Client * const client)
{
	t_replyBatch batch;
	const std::vector<std::string>& params = msg.getParams();

	if (params.size() < 2)
	{
		batch = obj->errNeedMoreParams(client, msg);
		return batch;
	}

	const std::string&	message = params[1];

	if (message.empty())
	{
		return batch;
	}
	
	std::vector<std::string>	target_vec = ft_std::split(params.at(0), ",");

	for (std::vector<std::string>::iterator it = target_vec.begin(); it != target_vec.end(); it++)
	{
		const std::string& target = *it;

		Message reply;

		reply.setSource(client->constructSource());
		reply.setCommand("PRIVMSG");
		reply.pushParam(target);
		reply.pushParam(message);

		if (target[0] == '#')
		{
			Channel* channel = obj->getChannelByName(target);

			if (!channel)
			{
				Message err;
				err.setSource(obj->getServerName());
				err.setCommand(ERR_NOSUCHCHANNEL);
				err.pushParam(client->getNickname());
				err.pushParam(target);
				err.pushParam("No such channel");

				t_reply reply_item;
				reply_item.first = client;
				reply_item.second.push(err);
				batch.push_back(reply_item);

				continue ;
			}

			if (!channel->hasThisClient(client))
			{
				Message err;
				err.setSource(obj->getServerName());
				err.setCommand(ERR_CANNOTSENDTOCHAN);
				err.pushParam(client->getNickname());
				err.pushParam(target);
				err.pushParam("Cannot send to channel");

				t_reply reply_item;
				reply_item.first = client;
				reply_item.second.push(err);
				batch.push_back(reply_item);

				continue;
			}
			
			obj->pushChannelReplyOthers(reply, channel, batch, client);
			continue ;

		}
		else
		{
			Client* target_client = obj->getClientByNickname(target);

			if (!target_client)
			{
				Message err;
				err.setSource(obj->getServerName());
				err.setCommand(ERR_NOSUCHNICK);
				err.pushParam(client->getNickname());
				err.pushParam(target);
				err.pushParam("No such nick/channel");

				t_reply reply_item;
				reply_item.first = client;
				reply_item.second.push(err);
				batch.push_back(reply_item);

				continue ;
			}

			t_reply reply_item;
			reply_item.first = target_client;
			reply_item.second.push(reply);
			batch.push_back(reply_item);

			continue ;
		}
	}

	return batch;
}
