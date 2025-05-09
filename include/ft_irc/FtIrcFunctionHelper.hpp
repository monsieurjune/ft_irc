/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrcFunctionHelper.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 00:13:05 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/20 12:37:09 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FT_IRC_FUNCTION_HELPER_HPP__
#define __FT_IRC_FUNCTION_HELPER_HPP__
#include "FtIrc.hpp"

/**
 * @brief Construct Reply Batch for reply back to Sender
 * 
 * @param reply_msg Setted Message
 * @param client Sender
 * 
 * @return Reply Batch for response back to sender
 */
FtIrc::t_replyBatch	singleReplySingleClientBatch(Message const & reply_msg, Client * const client);

/**
 * @brief Construct Reply Batch for reply back to Clients in Set
 * 
 * @param reply_msg Standard Message
 * @param clientSet Set of Client
 * 
 * @return Reply Batch for broadcast to clients in Set
 */
FtIrc::t_replyBatch singleReplyMultiClientBatch(Message const & reply_msg, std::set<Client*> const & clientSet);

#endif