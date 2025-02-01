/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircPASS.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 18:33:17 by scharuka          #+#    #+#             */
/*   Updated: 2025/02/01 14:04:11 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"

FtIrc::t_replyBatch	FtIrc::ircPASS(FtIrc * const obj, Message const & msg, Client * const client)
{
	std::vector<std::string> const&	params	= msg.getParams();

	// Invalid size of Params
	if (params.size() != 1)
	{
		
	}

	// Already Provide PASS
	if (client->containFlags(PASS_FLAG))
	{

	}

	if (params.at(1) != obj->getServerPassword())
	{
		
	}

	client->setAuthenLevel(PASS_FLAG);

	// Return 0 size vector
	return FtIrc::t_replyBatch();
}