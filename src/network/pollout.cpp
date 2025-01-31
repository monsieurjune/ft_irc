/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pollout.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:52:58 by tponutha          #+#    #+#             */
/*   Updated: 2025/01/31 18:10:48 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "network/network.hpp"
#include "ft_irc/FtIrc.hpp"

#include "exception/CstdException.hpp"
#include "exception/IrcDisconnectedException.hpp"

// C Header
#include <sys/errno.h>
#include <sys/socket.h>

namespace ft_net
{

void	pollout(FtIrc *main_obj, int fd, int revents)
{
	if (!(revents & POLLOUT))
	{
		return;
	}

	Client	*client = main_obj->getClientByFd(fd);

	if (client == NULL)
	{
		return;
	}

	try
	{
		if (client->countReply() == 0)
		{
			return;
		}
		
		std::string	msg	= client->dequeueReply();	

		if (send(fd, msg.c_str(), msg.length(), 0) < 0)
		{
			if (errno == EPIPE)
			{
				throw IrcDisconnectedException(
					"Client is disconnected when try to write"
				);
			}
			throw CstdException(errno);
		}
	}
	catch (const std::exception& e)
	{
		// Ignored
	}
}

}