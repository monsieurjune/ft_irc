/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc::ircMessageHandler.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 19:53:55 by tnualman          #+#    #+#             */
/*   Updated: 2025/02/01 21:10:47 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FtIrc.hpp"

std::string	ltoa(long num)
{
	std::ostringstream oss;

	oss << num;
	return oss.str();
}

// Tun has better version of this.
void FtIrc::ircMessageHandler(Message const & message, Client * const sender)
{
	for (int i = 0; i < _replies.size(); i++)
	{
		_replies.at(i).clear();
		_replies.at(i).resize(0);
	}	
	_replies.clear();
	_replies.resize(0);
	int	cmd_idx = -1;
	int	return_code = -1;
	
	while (++cmd_idx < 4) // _availableCommands.size() ; using literal value for now to prevent segfault.
		if (message.getCommand().compare(_availableCommands.at(cmd_idx)) == 0)
			return_code = (this->*_commandHandler[cmd_idx])(message, sender);

	// Put more code here in case of "command not found!".
	// if (return_code == -1)
	
	// sendRepliesToClient(sender);

	// return (return_code);
}

FtIrc::t_replyBatch FtIrc::err_NeedMoreParams(Message const & message, Client * const sender)
{
	Message			reply_msg;
	t_reply			reply;
	t_replyBatch	batch;
	
	reply_msg.setSource(_serverName);
	reply_msg.setCommand(ERR_NEEDMOREPARAMS);
	reply_msg.pushParam(sender->getNickname());
	reply_msg.pushParam(message.getCommand());
	reply_msg.pushParam("Not enough parameters");
	reply.first = sender;
	reply.second.push(reply_msg);
	batch.push_back(reply);
	
	return (batch);
}

FtIrc::t_replyBatch FtIrc::rpl_Topic_WhoTime(Message const & message, Client * const client, Channel * const channel)
{
	Message			reply_msg;
	t_reply			reply;
	t_replyBatch	batch;
	
	reply_msg.setSource(_serverName);
	reply_msg.setCommand(RPL_TOPIC);
	reply_msg.pushParam(client->getNickname());
	reply_msg.pushParam(channel->getName());
	reply_msg.pushParam(channel->getTopic());
	reply.first = client;
	reply.second.push(reply_msg);
	reply_msg.resetParams();
	reply_msg.setCommand(RPL_TOPICWHOTIME);
	reply_msg.pushParam(client->getNickname());
	reply_msg.pushParam(channel->getName());
	reply_msg.pushParam(channel->getTopicSetter());
	reply_msg.pushParam(ltoa(channel->getTimeTopicSet()));
	reply.second.push(reply_msg);
	batch.push_back(reply);
	
	return (batch);
}

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
