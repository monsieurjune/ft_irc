/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircReply.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 15:53:27 by tponutha          #+#    #+#             */
/*   Updated: 2025/02/01 16:48:13 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __IRC_REPLY_HPP__
#define __IRC_REPLY_HPP__

// Project Header
#include "FtIrcConstants.hpp"
#include "FtIrc.hpp"

// CPP Header
#include <string>

FtIrc::t_replyBatch	errNeedMoreParams(FtIrc * const obj, Client * const client, std::string const& cmd);

FtIrc::t_replyBatch	errUnknowCmd(FtIrc * const obj, Client * const client, std::string const& cmd);

FtIrc::t_replyBatch	errAlreadyRegistered(FtIrc * const obj, Client * const client);

FtIrc::t_replyBatch	errNotRegistered(FtIrc * const obj, Client * const client);

FtIrc::t_replyBatch	errInputTooLong(FtIrc * const obj, Client * const client);


#endif