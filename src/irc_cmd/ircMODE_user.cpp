/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircMODE_user.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 16:57:13 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/19 07:50:28 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "std/ft_cppstd.hpp"
#include "std/ft_cstd.hpp"

FtIrc::t_replyBatch	FtIrc::ircMODE_user(Message const & message, Client * const sender)
{
	std::vector<std::string> const &	params = message.getParams();
	std::string const 					target_name = params.at(0);
	Client * const 						target = getClientByNickname(target_name);
	Message								reply_msg;
	t_reply								reply_sender;
	t_replyBatch						batch;

	// Set Up Common Thing
	reply_msg.setSource(_serverName);
	reply_sender.first = sender;

	if (!target)
	{
		reply_msg.setCommand(ERR_NOSUCHNICK);
		reply_msg.pushParam(sender->getNickname());
		reply_msg.pushParam(target_name);
		reply_msg.pushParam("No such nickname.");
		reply_sender.second.push(reply_msg);
		batch.push_back(reply_sender);

		return (batch);
	}

	std::string							modestr = params.at(1);
	char								sign = modestr.at(0);
	std::map<int, Client*>::iterator	it_cmap;
	std::map<int, Client*>::iterator	it_cmap_end;

	for (std::string::iterator it = modestr.begin() + 1; it != modestr.end(); it++)
	{
		if (*it == '-' || *it == '+')
		{
			sign = *it;
			continue ;
		}

		reply_msg.setCommand("MODE");
		it_cmap = _clientMapByFd.begin();
		it_cmap_end = _clientMapByFd.end();

		switch (*it)
		{
			case (MODE_OPERATOR):
			{
				if (!sender->hasMode(MODE_OPERATOR))
				{		
					reply_msg.setCommand(ERR_NOPRIVILEGES);
					reply_msg.pushParam(sender->getNickname());
					reply_msg.pushParam("You're not an IRC operator.");
					reply_sender.second.push(reply_msg);
					continue ;
				}

				if (target->hasMode(MODE_OPERATOR) && sign == '-')
				{
					target->removeMode(MODE_OPERATOR);
					reply_msg.pushParam(target_name);
					reply_msg.pushParam("-o");
					pushServerReplyAll(reply_msg, batch);
					continue ;
				}

				if (!(target->hasMode(MODE_OPERATOR)) && sign == '+')
				{
					target->addMode(MODE_OPERATOR);
					reply_msg.pushParam(target_name);
					reply_msg.pushParam("+o");
					pushServerReplyAll(reply_msg, batch);
					continue ;
				}

			} break ;

			default:
			{
				reply_msg.setCommand(ERR_UNKNOWNMODE);
				reply_msg.pushParam(sender->getNickname());
				reply_msg.pushParam(std::string(1, *it));
				reply_msg.pushParam("is not a recognized user mode character.");
				reply_sender.second.push(reply_msg);
			}
		}
	}
	batch.push_back(reply_sender);

	return (batch);
}
