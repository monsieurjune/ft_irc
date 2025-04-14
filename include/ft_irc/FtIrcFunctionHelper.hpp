/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrcFunctionHelper.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 00:13:05 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/15 02:18:21 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FT_IRC_FUNCTION_HELPER_HPP__
#define __FT_IRC_FUNCTION_HELPER_HPP__
#include "FtIrc.hpp"

/**
 * @brief Push Nickname to Message's param with empty nickname handle
 * 
 * - <nickname> != '': Put <nickname>
 * 
 * - <nickname> == '': Put *
 * 
 * @param reply_msg Message
 * @param client Client
 * 
 * @warning Should be used to put Nickname at first param of Message
 */
void	nicknameMessageHelper(Message & reply_msg, Client * const client);

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