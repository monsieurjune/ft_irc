/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_recv.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:37:51 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/17 05:21:33 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Proiject Header
#include "network/network.hpp"
#include "std/ft_cstd.hpp"

// Custom Exception
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

	// Client is disconnected
	if (recv_len == 0)
	{
		throw IrcDisconnectedException(
				"Client is Disconnected while trying to receive msg"
			);
	}

	// There should be something to notify when use recv() wrong
	if (recv_len == -1)
	{
		throw CstdException(errno);
	}

	return recv_len;
}

static inline ssize_t	sb_check_n_replace_nul(char* buff, ssize_t recv_len)
{
	ssize_t	first_nul_pos	= -1;
	bool	isContainNUL	= false;

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

		// get first NUL position
		if (!isContainNUL)
		{
			first_nul_pos = i;
		}
		isContainNUL = true;
	}

	// Put proper NUL at the end of buffer
	buff[recv_len] = '\0';

	return first_nul_pos;
}

static inline char* sb_find_irc_terminate(int fd, char *buff, ssize_t recv_len)
{
	char*	terminate_pos	= NULL;

	terminate_pos = ft_std::strnstr(buff, IRC_TERMINATE_BYTES, recv_len);
	if (terminate_pos == NULL)
	{
		// Check if buff's length is at least 512 bytes or not
		if (recv_len >= IRC_MSG_MAXSIZE)
		{
			// Discard socket's message
			sb_recv(fd, buff, IRC_MSG_MAXSIZE, 0);
			throw IrcTooLongMsgException("Message is too long, Discarded");
		}

		// socket's buffer hasn't reach to the mature level yet
		throw IrcContinueException("This socket's buffer isn't mature yet");
	}

	return terminate_pos;
}

namespace ft_net
{

std::string	irc_recv(int fd)
{
	char	buff[IRC_MSG_MAXSIZE + 1];
	char*	terminate_pos	= NULL;
	ssize_t	recv_len		= 0;
	ssize_t	first_nul_pos	= -1;

	// Peek the Message first, Not outright remove it from socket's buffer
	recv_len = sb_recv(fd, buff, IRC_MSG_MAXSIZE, MSG_PEEK);

	// Replace NUL with valid character
	first_nul_pos = sb_check_n_replace_nul(buff, recv_len);

	// Find \r\n in message
	terminate_pos = sb_find_irc_terminate(fd, buff, recv_len);

	// Put \0 at terminate section to replace 'irc terminate' with 'c terminate'
	terminate_pos[0] = '\0';

	// Transform it into CPP string
	std::string	ret_msg(buff);

	// Check if proper message contain NUL
	if (first_nul_pos > -1 && static_cast<size_t>(first_nul_pos) < ret_msg.length())
	{
		sb_recv(fd, buff, ret_msg.length() + sizeof(IRC_TERMINATE_BYTES) - 1, 0);
		throw IrcInvalidPacketException("There is Invalid Character (NUL) in message");
	}

	// Check if proper msg contain standalone CR, LF
	size_t	cr_len	= ret_msg.find('\r');
	size_t	lf_len	= ret_msg.find('\n');

	if (cr_len != std::string::npos || lf_len != std::string::npos)
	{
		sb_recv(fd, buff, ret_msg.length() + sizeof(IRC_TERMINATE_BYTES) - 1, 0);
		throw IrcInvalidPacketException("There is Invalid Character (CR, LF) in message");
	}

	// There is no invaid message here now, so remove msg from socket's buffer
	sb_recv(fd, buff, ret_msg.length() + sizeof(IRC_TERMINATE_BYTES) - 1, 0);

	return ret_msg;
}

}