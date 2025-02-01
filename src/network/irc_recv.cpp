/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_recv.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:37:51 by tponutha          #+#    #+#             */
/*   Updated: 2025/02/01 12:35:44 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Proiject Header
#include "network/network.hpp"
#include "std/ft_cstd.hpp"
#include "exception/CstdException.hpp"
#include "exception/IrcInvalidPacketException.hpp"
#include "exception/IrcTooLongMsgException.hpp"
#include "exception/IrcDisconnectedException.hpp"
#include "exception/IrcContinueException.hpp"

// C Header
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/errno.h>

// CPP Header
#include <string>

static inline ssize_t	sb_recv(int fd, void *buff, size_t len, int flags)
{
	ssize_t	recv_len = recv(fd, buff, len, flags);

	if (recv_len == 0)
	{
		// Client is disconnected
		throw IrcDisconnectedException(
				"Client is Disconnected while trying to receive msg"
			);
	}

	if (recv_len == -1)
	{
		// Client's socket is error
		throw CstdException(errno);
	}

	return recv_len;
}

static inline void	sb_replace_nul(char* buff, ssize_t recv_len)
{
	// Replace NUL with CR or LF
	// All of them is invalid to stand alone
	// But NUL is terminate character of C/C++

	for (ssize_t i = 0; i < recv_len; i++)
	{
		// if this character is not NUL, just skip
		if (buff[i] != '\0')
		{
			continue;
		}
		
		// Avoid creating irc's terminate (\r\n)
		if (buff[i + 1] == '\r' || buff[i + 1] == '\n')
		{
			buff[i] = buff[i + 1];
		}
		else
		{
			buff[i] = '\r';
		}
	}

	// Put proper NUL at the end of buffer
	buff[recv_len] = '\0';
}

namespace ft_net
{

std::string	irc_recv(int fd)
{
	char	buff[IRC_MSG_MAXSIZE + 1];
	char*	terminate_pos	= NULL;
	ssize_t	recv_len		= 0;

	// Peek the Message first, Not outright remove it from socket's buffer
	recv_len = sb_recv(fd, buff, IRC_MSG_MAXSIZE, MSG_PEEK);

	// Replace NUL with other invalid character and put NUL for safety
	sb_replace_nul(buff, recv_len);

	// Find \r\n in message, otherwise ignored
	terminate_pos = ft_std::strnstr(buff, IRC_TERMINATE_BYTES, recv_len);
	if (terminate_pos == NULL)
	{
		// Check if buff's length is at least 512 bytes or not
		if (recv_len >= IRC_MSG_MAXSIZE)
		{
			sb_recv(fd, buff, IRC_MSG_MAXSIZE, 0); // Discard socket's message
			throw IrcTooLongMsgException("Message might be too long, Discarded");
		}

		// socket's buffer hasn't reach to the mature level yet
		throw IrcContinueException("This socket's buffer isn't mature yet");
	}

	// Put \0 at terminate section to replace irc terminate with c terminate
	terminate_pos[0] = '\0';

	// Transform it into CPP string
	std::string	ret_msg(buff);

	// Check if msg contain standalone \r, \n
	size_t	cr_len	= ret_msg.find('\r');
	size_t	lf_len	= ret_msg.find('\n');

	if (cr_len != std::string::npos || lf_len != std::string::npos)
	{
		sb_recv(fd, buff, ret_msg.length() + sizeof(IRC_TERMINATE_BYTES) - 1, 0);
		throw IrcInvalidPacketException(
				"There is Invalid Character (NUL, CR, LF) in message"
			);
	}

	// There is no invaid message here now, so remove msg from socket's buffer
	sb_recv(fd, buff, ret_msg.length() + sizeof(IRC_TERMINATE_BYTES) - 1, 0);
	
	return ret_msg;
}

}