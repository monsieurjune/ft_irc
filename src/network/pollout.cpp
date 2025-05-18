/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pollout.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:52:58 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/18 07:40:34 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "network/network.hpp"
#include "ft_irc/FtIrc.hpp"
#include "utils/ft_utils.hpp"

// Custom Exception
#include "exception/IrcDisconnectedException.hpp"

// C Header
#include <sys/socket.h>

namespace ft_net
{

void	pollout(FtIrc *main_obj, int fd, int revents)
{
	if (!(revents & POLLOUT))
	{
		return;
	}

	// Get Client
	Client	*client = main_obj->getClientByFd(fd);

	// Shouldn't happend
	if (client == NULL)
	{
		return;
	}

	try
	{
		// Check queue
		if (client->countReply() == 0)
		{
			return;
		}

		// Get string
		std::string	msg	= client->getFrontReply();

		// If send return -1, then let other part of program to check disconnect
		ssize_t n = send(fd, msg.c_str(), msg.length(), 0);
		if (n < 0)
		{
			return;
		}

		// trim or pop msg out
		client->dequeueReply(static_cast<size_t>(n));
	}
	catch (std::exception const& e)
	{
		ft_utils::logger(ft_utils::ERROR, "pollout", e.what());
	}
}

}