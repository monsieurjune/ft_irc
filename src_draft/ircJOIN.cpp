/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircJOIN.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 19:11:58 by tnualman          #+#    #+#             */
/*   Updated: 2025/02/05 17:04:48 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FtIrc.hpp"

std::string	ltoa(long num)
{
	std::ostringstream oss;

	oss << num;
	return oss.str();
}

std::vector<std::string> split(std::string const & s, std::string const & delimiter) 
{
	std::string tmp = s;
	std::vector<std::string> tokens;
	size_t pos = 0;
	std::string token;
	while ((pos = tmp.find(delimiter)) != std::string::npos)
	{
		token = tmp.substr(0, pos);
		tokens.push_back(token);
		tmp.erase(0, pos + delimiter.length());
	}
	tokens.push_back(tmp);
	return (tokens);
}

FtIrc::t_replyBatch FtIrc::ircJOIN(FtIrc * const obj, Message const & message, Client * const sender)
{
	std::vector<std::string>	params = message.getParams();
	Message						reply_msg;
	t_reply						reply_sender;
	t_replyBatch				batch;

	if (params.size() < 1 || params.at(0).empty())
	{
		return (obj->err_NeedMoreParams(message, sender));
	}
	
	// Special case: leaving all channels. (WIP)
	if (params.at(0) == "0")
	{
		return ;
	}

	reply_sender.first = sender;

	std::vector<std::string>	channel_name_vec = split(params.at(0), ",");
	std::vector<std::string>	channel_key_vec;
	bool						has_key_param = false;
	size_t						idx;
	
	// Assigning channel_key_vec.
	// and matching numbers of chennel and keys in vectors by adding empty strings to channel missing keys.
	if (params.size() < 2 || params.at(1).empty())
	{
		idx = 0;
	}
	else
	{
		channel_key_vec = split(params.at(1), ",");
		idx = channel_key_vec.size();
	}
	for (idx; idx < channel_name_vec.size(); idx++)
	{
		channel_key_vec.push_back("");
	}

	// Primary for loop here.
	for (idx = 0; idx < channel_name_vec.size(); idx++)
	{
		std::string	channel_name = channel_name_vec.at(idx);
		Channel *	channel = channel_name.empty() ? NULL : obj->getChannelByName(channel_name);

		if (!channel)
		{
			reply_msg.setSource(obj->_serverName);
			reply_msg.setCommand(ERR_NOSUCHCHANNEL);
			reply_msg.pushParam(sender->getNickname());
			reply_msg.pushParam(channel_name.empty() ? "*" : "#" + channel_name);
			reply_msg.pushParam("No such channel.");
			reply_sender.second.push(reply_msg);
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
			reply_msg.pushParam("#" + channel_name);
			reply_msg.pushParam("Cannot join channel (+l).");
			reply_sender.second.push(reply_msg);
			continue ;
		}

		if (channel->hasThisMode(MODE_INVITEONLY) && !(channel->isClientInvited(sender)))
		{
			reply_msg.setSource(obj->_serverName);
			reply_msg.setCommand(ERR_INVITEONLYCHAN);
			reply_msg.pushParam(sender->getNickname());
			reply_msg.pushParam("#" + channel_name);
			reply_msg.pushParam("Cannot join channel (+i).");
			reply_sender.second.push(reply_msg);
			continue ;
		}

		if (channel->hasThisMode(MODE_CHANNELKEY) && channel_key_vec.at(idx) != channel->getPassword())
		{
			reply_msg.setSource(obj->_serverName);
			reply_msg.setCommand(ERR_BADCHANNELKEY);
			reply_msg.pushParam(sender->getNickname());
			reply_msg.pushParam("#" + channel_name);
			reply_msg.pushParam("Cannot join channel (+k).");
			reply_sender.second.push(reply_msg);
			continue ;
		}

		// General/valid joining case here.
		{
			channel->addUserToChannel(sender, "");
			// Replies to sender
		}
	}

	batch.push_back(reply_sender);
	return (batch);
}
