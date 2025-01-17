/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc_ircTOPIC.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 01:26:39 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/17 22:22:43 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FtIrc.hpp"

int FtIrc::ircTOPIC(Message const & message, Client * const sender)
{
	int param_count = message.getParams().size();
	std::stringstream details;
	
	if (param_count < 1)
	{
		details << "TOPIC :Not enough parameters";
		return (addReplyMessage(ERR_NEEDMOREPARAMS, sender, details.str()));
	}

	std::string	channel_name = message.getParams().at(0);
	Channel* channel = getChannelByName(channel_name);
	
	if (!channel)
	{
		details << channel_name << " :No such channel";
		return (addReplyMessage(ERR_NOSUCHCHANNEL, sender, details.str()));
	}

	if (param_count == 1)
	{
		if (channel->getTopic().empty())
		{
			details << channel_name << " :No topic is set";
			return (addReplyMessage(RPL_NOTOPIC, sender, details.str()));
		}
		else
		{
			details << channel_name + " :" + channel->getTopic();
			addReplyMessage(RPL_TOPIC, sender, details.str());
			details.clear();
			details << channel_name << " " << channel->getTopicSetter() << " " << channel->getTimeTopicSet();
			addReplyMessage(RPL_TOPICWHOTIME, sender, details.str());
			return (RPL_TOPIC);
		}
	}

	if (!channel->hasUser(sender))
	{
		details << channel_name << " :You're not on that channel";
		return (addReplyMessage(ERR_NOTONCHANNEL, sender, details.str()));
	}

	if (channel->hasMode('t') && !channel->hasMembershipMode(sender, 'o'))
	{details << channel_name + " :" + channel->getTopic();
			addReplyMessage(RPL_TOPIC, sender, details.str());
			details.clear();
			details << channel_name << " " << channel->getTopicSetter() << " " << channel->getTimeTopicSet();
			addReplyMessage(RPL_TOPICWHOTIME, sender, details.str());
			return (RPL_TOPIC);
		details << channel_name << " :You're not on channel operator";
		return (addReplyMessage(ERR_CHANOPRIVSNEEDED, sender, details.str()));
	}
	
	channel->setTopic(message.getParams().at(1), sender);
	
	// TODO: Write code to broadcast ":<server> TOPIC <topic>" to every user on the channel here!!
	
	details << channel_name + " " + channel->getTopic();
	return (addReplyMessage(RPL_TOPIC, sender, details.str()));
}
