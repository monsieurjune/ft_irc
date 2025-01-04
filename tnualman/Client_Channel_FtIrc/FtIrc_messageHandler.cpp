/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc_messageHandler.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 19:53:55 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/05 01:30:41 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FtIrc.hpp"

int FtIrc::ircMessageHandler(Message const & message, Client const * const sender, std::string & output)
{
	int	cmd_idx = -1;
	while (++cmd_idx < _commandVec.size())
		if (message.getCommand().compare(_commandVec.at(cmd_idx)) == 0)
			break ;

	switch (cmd_idx)
	{
		case (0): // KICK
			// KICK
			break ;
		case (1): // INVITE
			// INVITE
			break ;
		case (2): // MODE
			// MODE
			break ;
		case (3): // TOPIC
			// TOPIC
			return (FtIrc::ircCommandTOPIC(message, sender, output));
			break;
		default:
			// Command does not exist!
			return (-1);
	}
	return (0);
}

int FtIrc::ircCommandMODE(Message const & message, Client const * const sender, std::string & output)
{
	return (0);
}

int FtIrc::ircCommandTOPIC(Message const & message, Client const * const sender, std::string & output)
{
	int param_count = message.getParams().size();

	if (param_count < 1 || param_count > 2)
	{
		output = "Invalid number of parameters for TOPIC command!";
		// We should define error macros, enum, or exception ...
		return (1);
	}

	std::string	channel_name = message.getParams().at(0);
	Channel* channel = getChannelByName(channel_name);
	
	if (!channel)
	{
		output = "Channel named " + channel_name + " not found!";
		// We should define error macros, enum, or exception
		return (2);
	}

	if (param_count == 1)
	{
		output = "Channel: " + channel_name + "; Topic: " + channel->getTopic();
		// Put code to send the message to client, either here on in a network handler function ...
		return (0);
	}
	
	// if (NOT FOUND IN CHANNEL USER LIST || NOT AN OPERATOR OF THE CHANNEL)
	// {
	// 	*output = "User " + sender->getNickname() + " cannot set the topic for channel " + channel_name + " !";
	// 	// Put code to send the message to client, either here on in a network handler function ...
	// 	// We should define error macros, enum, or exception
	// 	return (3);
	// }
	
	channel->setTopic(message.getParams().at(1), sender);
	output = "Channel: " + channel_name + "; Topic: " + channel->getTopic() + "; Setter: " + channel->getTopicSetter();
	// Put code to send the message to client, either here on in a network handler function ...
	
	return (0);
}
