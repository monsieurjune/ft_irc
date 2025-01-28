/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc::ircMODE.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 22:04:23 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/28 20:13:41 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FtIrc.hpp"

FtIrc::t_replyBatch FtIrc::ircMODE(FtIrc * const obj, Message const & message, Client * const sender)
{
	std::vector<std::string> const &	params = message.getParams();
	Message								reply_msg;
	t_reply								reply;
	t_replyBatch						batch;

	if (params.size() == 0)
	{
		return (obj->err_NeedMoreParams(message, sender));
	}

	try
	{
		if (params.at(0).at(0) == '#')
		{
			return (obj->ircMODE_channel(message, sender));
		}
		else
		{
			return (obj->ircMODE_user(message, sender));
		}
	}
	catch(const std::exception& e)
	{
		return (obj->err_NeedMoreParams(message, sender));
	}
}

int FtIrc::ircMODE_channel(Message const & message, Client * const sender)
{
	std::vector<std::string> const & params = message.getParams();
	std::string const target = params.at(0).substr(1, std::string::npos);
	Channel * const channel = getChannelByName(target);
	
	if (channel == NULL)
	{
		//_replies.push_back(":" + _serverName + " :" + sender->getNickname() + " " + target + " : No such channel"); 
		return (ERR_NOSUCHCHANNEL);
	}

	if (params.size() == 1)
	{
		_replies.push_back("");
		return (0);
	}
	return (0);
}
