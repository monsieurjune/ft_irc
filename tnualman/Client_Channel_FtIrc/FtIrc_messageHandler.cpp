/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc_messageHandler.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 19:53:55 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/14 20:11:11 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FtIrc.hpp"

int FtIrc::ircMessageHandler(Message const & message, Client * const sender)
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
	
	ircReplyToClient(sender);

	return (return_code);
}

/**
 * @brief This function is to be called only inside the function FtIrc::ircMessageHandler!
 * It sends all the reply messages collected into std::vector<std::string> _replies 
 * by the function FtIrc::ircAddReplyMessage.
 * 
 * TODO for Tun and Grammy: Add code that actually send the replies to the cliient.
 * 
 * @param sender 
 */
void FtIrc::ircReplyToClient(Client * const sender)
{
	/**
	 * @brief This function is to be called only inside the function FtIrc::ircMessageHandler!
	 * It sends all the reply messages collected into std::vector<std::string> _replies 
	 * by the function FtIrc::ircAddReplyMessage.
	 * 
	 * TODO for Tun and Grammy: Add code that actually send the replies to the cliient.
	 * 
	 * @param sender 
	 */
}

void FtIrc::ircAddReplyMessage(int const code, std::string const & target, std::string const & detail)
{
	std::stringstream ss;
	
	ss << ":" << _serverName << " " << code << " " << target << " :" << detail;
	
	_replies.push_back(ss.str());
}

int FtIrc::ircCommandTOPIC(Message const & message, Client * const sender)
{
	int param_count = message.getParams().size();

	if (param_count < 1 || param_count > 2)
	{
		_replies.push_back("Invalid number of parameters for TOPIC command!");
		// TODO: replace return value with correct enum from e_numerics.
		return (1);
	}

	std::string	channel_name = message.getParams().at(0);
	Channel* channel = getChannelByName(channel_name);
	
	if (!channel)
	{
		_replies.push_back("Channel named " + channel_name + " not found!");
		// TODO: replace return value with correct enum from e_numerics.
		return (2);
	}

	// No changing topic in this case; only reply to client the topic string. 
	if (param_count == 1)
	{
		_replies.push_back("Channel: " + channel_name + "; Topic: " + channel->getTopic());
		// TODO: replace return value with correct enum from e_numerics.
		return (0);
	}

	// if (NOT FOUND IN CHANNEL USER LIST || NOT AN OPERATOR OF THE TOPIC-PROTECTED CHANNEL)
	if (!channel->hasUser(sender) || (channel->hasMode('t') && !channel->hasMembershipMode(sender, 'o')))
	{
		_replies.push_back("User " + sender->getNickname() + " cannot set the topic for channel " + channel_name + " !");
		// TODO: replace return value with correct enum from e_numerics.
		return (3);
	}
	
	channel->setTopic(message.getParams().at(1), sender);
	_replies.push_back("Channel: " + channel_name + "; Topic: " + channel->getTopic() + "; Setter: " + channel->getTopicSetter());
	
	// TODO: replace return value with correct enum from e_numerics.
	return (0);
}
