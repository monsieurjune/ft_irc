/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircUSER.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 18:33:17 by scharuka          #+#    #+#             */
/*   Updated: 2025/05/17 20:16:46 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"

// <username> 0 * <realname>
// <username> <hostname> <servername> <realname>

FtIrc::t_replyBatch	FtIrc::ircUSER(FtIrc * const obj, Message const & msg, Client * const client)
{
	std::vector<std::string> const&	params  = msg.getParams();

    if (params.size() != 4)
    {
        return obj->errNeedMoreParams(client, msg);
    }

    if (client->containFlags(USER_FLAG))
    {
        return obj->errAlreadyRegistered(client);
    }

    // Get username & realname (hostname & servername is ignored here)
    std::string const&  username    = params.at(0);
    std::string const&  realname    = params.at(3);

    client->setUsername(username.substr(0, USERLEN));
    client->setRealname(realname);
    client->setAuthenLevel(client->getAuthenLevel() | USER_FLAG);

    if (client->containFlags(PASS_FLAG | USER_FLAG | NICK_FLAG))
    {
        client->setAuthenLevel(client->getAuthenLevel() | LOGIN_FLAG);

        return obj->rplWelcome(client);
    }

    return FtIrc::t_replyBatch(); // Return Nothing
}