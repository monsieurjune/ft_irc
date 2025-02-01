/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errUnknownCmd.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 05:45:02 by tponutha          #+#    #+#             */
/*   Updated: 2025/02/01 16:48:21 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc/ircReply.hpp"

FtIrc::t_replyBatch	errUnknowCmd(FtIrc * const obj, Client * const client, std::string const& cmd)
{
	Message	reply_msg;

	// Creating MSG
	reply_msg.setSource(obj->getServerName());
	reply_msg.setCommand(ERR_UNKNOWNCOMMAND);
	if (client->getNickname().empty())
	{
		reply_msg.pushParam("*");
	}
	else
	{
		reply_msg.pushParam(client->getNickname());
	}
	reply_msg.pushParam(cmd);
	reply_msg.pushParam("Unknown command");

	// Put it in proper struct
	std::queue<Message>	queue;

	queue.push(reply_msg);

	FtIrc::t_reply		reply(client, queue);
	FtIrc::t_replyBatch	batch(1, reply);

	return batch;
}