/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircMODE.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 22:04:23 by tnualman          #+#    #+#             */
/*   Updated: 2025/04/13 22:01:35 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"

FtIrc::t_replyBatch FtIrc::ircMODE(FtIrc * const obj, Message const & message, Client * const sender)
{
	std::vector<std::string> const &	params = message.getParams();

	if (params.size() == 0 || params.at(0).empty())
	{
		return (obj->errNeedMoreParams(sender, message));
	}

	if (params.at(0).at(0) == '#')
	{
		return (obj->ircMODE_channel(message, sender));
	}
	else
	{
		return (obj->ircMODE_user(message, sender));
	}
}