/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircQUIT.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 18:33:17 by scharuka          #+#    #+#             */
/*   Updated: 2025/05/17 20:27:17 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"

FtIrc::t_replyBatch	FtIrc::ircQUIT(FtIrc * const obj, Message const & msg, Client * const client)
{
    std::vector<std::string>    params  = msg.getParams();

    std::string reason = (params.size() > 0) ? params.at(0) : "";

    obj->notifyQuitOnThisClient(client->getFd(), reason);

    return FtIrc::t_replyBatch();
}
