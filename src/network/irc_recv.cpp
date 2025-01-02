// Proiject Header
#include "network/ft_irc.hpp"
#include "network/network.hpp"
#include "std/ft_cstd.hpp"

#include "exception/CstdException.cpp"
#include "exception/IrcInvalidPacketException.hpp"
#include "exception/IrcDisconnectedException.hpp"

// C Header
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/errno.h>

// CPP Header
#include <string>

static inline void	sb_check_recv_error(ssize_t recv_len)
{
	if (recv_len == 0)
	{
		throw IrcDisconnectedException("Client is Disconnected while trying to read");
	}

	if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
	{
		return;
	}
	throw CstdException(errno);
}

namespace ft_net
{

bool	irc_recv(int fd, std::string& msg)
{
	char 	buff[IRC_MAXSIZE + 1];
	char	*term_pos	= NULL;
	ssize_t	recv_len	= 0;
	ssize_t	actual_len	= 0;

	// Put '\0' at the end of buffer for safety and not mendle with irc rule
	buff[IRC_MAXSIZE] = '\0';

	// Peek the Message first, Not outright remove it from socket's buffer
	recv_len = recv(fd, buff, IRC_MAXSIZE, MSG_PEEK);
	if (recv_len <= 0)
	{
		sb_check_recv_error(recv_len);
		return false;
	}

	// Check for \r\n to get terminate
	// if not found \r\n then check if msg is already at 510 bytes or not
	// 	- if it would likely to exceed 510 bytes, then give error
	// 	- else, just exit and wait msg to fulfill buffer
	term_pos = ft_std::strnstr(buff, IRC_TERMINATE, recv_len);
	if (term_pos == NULL)
	{
		if (recv_len > IRC_MAXSIZE - 2)
		{
			throw IrcInvalidPacketException("This Client's Message doesn\'t has proper terminate bytes");
		}
		return false;
	}

	// Assume that this msg is contained proper \r\n
	// Check for \0 that mixed in msg
	if (ft_std::strcnlen(buff, 0, recv_len) < recv_len)
	{
		throw IrcInvalidPacketException("This Client's Message is contained with 0x00 charactor");
	}

	// Construct buffer into String and replace \r\n with \0
	term_pos[0] = '\0';
	msg.assign(buff, actual_len);

	// Get Actual length of IRC's Message (take \0 pos that replace \r\n)
	actual_len = msg.length();

	// There is Only Valid IRC MSG Here

	// Clear IRC MSG from socket's buffer
	// Assume that it must has anything to read here
	if (recv(fd, buff, actual_len + 2, 0) <= 0)
	{
		sb_check_recv_error(actual_len);
		return false;
	}

	return true;
}

}