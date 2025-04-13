/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircPRIVMSG.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 18:36:40 by scharuka          #+#    #+#             */
/*   Updated: 2025/04/13 23:14:54 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"

FtIrc::t_replyBatch FtIrc::ircPRIVMSG(FtIrc * const obj, Message const & msg, Client * const client)
{
    (void)obj;
    (void)msg;
    (void)client;

    return FtIrc::t_replyBatch();
}