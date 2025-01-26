/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc::ircMessageHandler.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 19:53:55 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/26 21:11:54 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FtIrc.hpp"

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

// /**
//  * @brief This function is to be called only inside the function FtIrc::ircMessageHandler!
//  * It sends all the reply messages collected into std::vector<std::string> _replies 
//  * by the function FtIrc::ircAddReplyMessage.
//  * 
//  * TODO for Tun and Grammy: Add code that actually send the replies to the client.
//  * 
//  * @param sender 
//  */
// int FtIrc::sendRepliesToClient(Client * const sender)
// {
// 	/**
// 	 * @brief This function is to be called only inside the function FtIrc::ircMessageHandler!
// 	 * It sends all the reply messages collected into std::vector<std::string> _replies 
// 	 * by the function FtIrc::ircAddReplyMessage.
// 	 * 
// 	 * TODO for Tun and Grammy: Add code that actually send the replies to the client.
// 	 * 
// 	 * @param sender 
// 	 */

// 	try
// 	{
// 		Message firstMsg(_replies.at(0));
		
// 		return (std::stoi(firstMsg.getCommand().c_str()));
// 	}
// 	catch(const std::exception& e) // No reply is sent to client.
// 	{
// 		return (0);
// 	}
// }

// /**
//  * @brief This fucntion is to be called inside IRC command handler functions,
//  * 	to generate each complete IRC reply message and put them inside std::vector<std::string> _replies,
//  * 	to be sent successively to the client with the function FtIrc::ircReplyToClient.
//  * 
//  * @param code is the IRC reply code.
//  * @param sender is the sender of the IRC command message to the server,
//  * 	passing through from the function FtIrc::ircMessageHandler.
//  * @param details is the string containing parameters of the reply,
//  * 	to be constructed in different places by the different reply formats and logics of each handler function and ease of its case.
//  * @return the same IRC reply code (int)
//  * 
//  * Note: The types of 'details' and the implementation logic keep changing between std::string and std::stringstream
//  * 	to accomodate int/long to std::string conversion, and also for an obscure c++98 inability to use an std::stringstream
//  * 	as an argument for the function; I really don't understand this part! 
//  */

// int	FtIrc::addReplyMessage(int const code, Client * const sender, std::string const & details)
// {
// 	std::stringstream ss;
	
// 	ss << ":" << _serverName << " " << code << " " << sender->getNickname();

// 	if (!details.empty())
// 	{
// 		ss << " " << details;
// 	}
	
// 	_replies.push_back(ss.str());

// 	return (code);
// }
