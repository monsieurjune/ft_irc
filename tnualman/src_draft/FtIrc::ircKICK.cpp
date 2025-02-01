/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc::ircKICK.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 15:51:16 by tnualman          #+#    #+#             */
/*   Updated: 2025/02/01 21:30:33 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FtIrc.hpp"

std::string	ltoa(long num)
{
	std::ostringstream oss;

	oss << num;
	return oss.str();
}

std::vector<std::string> split(std::string const & s, std::string const & delimiter) 
{
    std::string tmp = s;
	std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = tmp.find(delimiter)) != std::string::npos)
	{
        token = tmp.substr(0, pos);
        tokens.push_back(token);
        tmp.erase(0, pos + delimiter.length());
    }
    tokens.push_back(tmp);

    return (tokens);
}

// WIP

// FtIrc::t_replyBatch FtIrc::ircKICK(FtIrc * const obj, Message const & message, Client * const sender)
// {
// 	std::vector<std::string>    params = message.getParams();
// 	Message			            reply_msg;
// 	t_reply			            reply_sender;
// 	t_replyBatch	            batch;


	
// 	if (params.size() < 2 || params.at(0).empty() || params.at(1).empty())
// 	{
// 		return (obj->err_NeedMoreParams(message, sender));
// 	}

//     std::string channel_name = params.at(0);
//     Channel *   channel = obj->getChannelByName(channel_name);
    
//     if (!channel)
//     {
//         reply_msg.setSource(obj->getServerName());
//         reply_msg.setCommand(ERR_NOSUCHCHANNEL);
//     }

// }
