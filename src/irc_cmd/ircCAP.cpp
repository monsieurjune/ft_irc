/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircCAP.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 18:33:17 by scharuka          #+#    #+#             */
/*   Updated: 2025/04/13 23:16:07 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"

FtIrc::t_replyBatch	FtIrc::ircCAP(FtIrc * const obj, Message const & msg, Client * const client)
{
	// Since this server wouldn't like to support capability for now
	(void)obj;
	(void)msg;
	(void)client;

	return FtIrc::t_replyBatch();
}