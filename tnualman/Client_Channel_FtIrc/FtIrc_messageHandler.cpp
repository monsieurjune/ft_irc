/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc_messageHandler.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 19:53:55 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/12 21:58:26 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FtIrc.hpp"

int FtIrc::ircMessageHandler(Message const & message, Client * const sender, std::string & output)
{
	int	cmd_idx = -1;
	while (++cmd_idx < 4) // _availableCommands.size() ; using literal value for now to prevent segfault.
		if (message.getCommand().compare(_availableCommands.at(cmd_idx)) == 0)
			return ((this->*_commandHandler[cmd_idx])(message, sender, output));

	// Put more code here in case of "command not found!".
	return (-1);
}

int FtIrc::ircCommandMODE(Message const & message, Client * const sender, std::string & output)
{
	std::vector<std::string> const & params = message.getParams();

	if (params.size() == 0)
	{
		output = "No parameters given for MODE command!";
		// Find the correct return code from e_numerics.hpp .
		return (-2);
	}

	try
	{
		if (params.at(0).at(0) == '#')
		{
			return (ircCommandMODE_channel(message, sender, output));
		}
		else
		{
			return (ircCommandMODE_user(message, sender, output));
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << "Message parsing error: empty string for <target>!" << std::endl;
		output = "No parameters given for MODE command!";
		// Find the correct return code from e_numerics.hpp .
		return (-2);
	}
}

int FtIrc::ircCommandTOPIC(Message const & message, Client * const sender, std::string & output)
{
	int param_count = message.getParams().size();

	if (param_count < 1 || param_count > 2)
	{
		output = "Invalid number of parameters for TOPIC command!";
		// TODO: replace return value with correct enum from e_numerics.
		return (1);
	}

	std::string	channel_name = message.getParams().at(0);
	Channel* channel = getChannelByName(channel_name);
	
	if (!channel)
	{
		output = "Channel named " + channel_name + " not found!";
		// TODO: replace return value with correct enum from e_numerics.
		return (2);
	}

	// No changing topic in this case; only reply to client the topic string. 
	if (param_count == 1)
	{
		output = "Channel: " + channel_name + "; Topic: " + channel->getTopic();
		// Put code to send the message to client, either here on in a network handler function ...
		// TODO: replace return value with correct enum from e_numerics.
		return (0);
	}

	// if (NOT FOUND IN CHANNEL USER LIST || NOT AN OPERATOR OF THE TOPIC-PROTECTED CHANNEL)
	if (!channel->hasUser(sender) || (channel->hasMode('t') && !channel->hasMembershipMode(sender, 'o')))
	{
		output = "User " + sender->getNickname() + " cannot set the topic for channel " + channel_name + " !";
		// Put code to send the message to client, either here on in a network handler function ...
		// TODO: replace return value with correct enum from e_numerics.
		return (3);
	}
	
	channel->setTopic(message.getParams().at(1), sender);
	output = "Channel: " + channel_name + "; Topic: " + channel->getTopic() + "; Setter: " + channel->getTopicSetter();
	// Put code to send the message to client, either here on in a network handler function ...
	
	// TODO: replace return value with correct enum from e_numerics.
	return (0);
}
