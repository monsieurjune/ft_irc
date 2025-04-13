/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rplWhoReply.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:35:05 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/13 21:41:29 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "std/ft_cppstd.hpp"

FtIrc::t_replyBatch FtIrc::rplWhoReply(Channel * const channel)
{
    (void)channel;

    return t_replyBatch();
}