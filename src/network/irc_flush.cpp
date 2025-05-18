/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_flush.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:47:13 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/17 21:05:00 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Proiject Header
#include "network/network.hpp"
#include "std/ft_cstd.hpp"

// C Header
#include <sys/types.h>
#include <sys/socket.h>

static inline void  sb_replace_nul(char *buff, ssize_t recv_len)
{
    // Replace NUL with 0 (since if NUL is existed, whole msg is useless)
	for (ssize_t i = 0; i < recv_len; i++)
	{
		// if this character is not NUL, just skip
		if (buff[i] != '\0')
		{
			continue;
		}

		// Replace NUL with 0
		buff[i] = '0';
	}

	// Put proper NUL at the end of buffer
	buff[recv_len] = '\0';
}

namespace ft_net
{

int irc_flush(int fd)
{
	char    buff[IRC_MSG_MAXSIZE + 1];
	ssize_t recv_len        = 0;
	char*	terminate_pos	= NULL;

    // Peek the Message first, Not outright remove it from socket's buffer
	recv_len = ft_net::recv_wrapper(fd, buff, IRC_MSG_MAXSIZE, MSG_PEEK);

    // Replace NUL with valid character
	sb_replace_nul(buff, recv_len);

	// Find \r\n in message
	terminate_pos = ft_std::strnstr(buff, IRC_TERMINATE_BYTES, recv_len);

    // if not find, then just flush with <recv_len> amount
    if (terminate_pos == NULL)
    {
        // Check if last one is \r
        if (buff[recv_len - 1] == '\r')
        {
            ft_net::recv_wrapper(fd, buff, static_cast<size_t>(recv_len) - 1, 0);
        }
        else
        {
            ft_net::recv_wrapper(fd, buff, static_cast<size_t>(recv_len), 0);
        }

        return 1;
    }

    // else, just find \r\n and flush with its actual length
    // Put \0 at terminate section to replace 'irc terminate' with 'c terminate'
	terminate_pos[0] = '\0';

    // Transform it into CPP string
	std::string	ret_msg(buff);

    // We don't give a thing on too long msg's content, so remove msg from socket's buffer
	ft_net::recv_wrapper(fd, buff, ret_msg.length() + sizeof(IRC_TERMINATE_BYTES) - 1, 0);

    return 0;
}

}