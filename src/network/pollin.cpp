/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pollin.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:52:55 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/18 06:43:49 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Message.hpp"
#include "network/network.hpp"
#include "utils/ft_utils.hpp"

// Custom Exception
#include "exception/IrcContinueException.hpp"
#include "exception/IrcInvalidPacketException.hpp"
#include "exception/IrcTooLongMsgException.hpp"
#include "exception/IrcDisconnectedException.hpp"

// C Header
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static inline void	sb_accept_new_client(FtIrc *main_obj, int listen_fd)
{
	sockaddr_in	client_addr;
	socklen_t	client_addr_len = sizeof(client_addr);
	int			client_fd;
	char		*client_ip;

	client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
	if (client_fd < 0)
	{
		return;
	}

	client_ip = inet_ntoa(client_addr.sin_addr);
	main_obj->addClient(client_fd, client_ip);
}

static inline void	sb_reading(FtIrc *main_obj, Client *client)
{
	std::string	raw_msg = ft_net::irc_recv(client->getFd());
	Message		msg(raw_msg);

	// message format is wrong
	if (!msg.isValid())
	{
		return;
	}

	// handle message
	main_obj->ircMessageHandler(msg, client);
}

static inline void	sb_read_msg_from_client(FtIrc *main_obj, int fd)
{
	Client*	client = main_obj->getClientByFd(fd);

	// Client is not exist (shouldn't happend)
	if (client == NULL)
	{
		return;
	}

	// Check flsuhing status
	if (client->getIsFlushing())
	{
		if (ft_net::irc_flush(client->getFd()) == 0)
		{
			client->resetIsFlushing();
		}
		return;
	}

	// run normal thing
	sb_reading(main_obj, client);
}


static inline void	sb_handle_too_long(FtIrc *main_obj, int fd, std::string const& msg)
{
	Client*	ptr = main_obj->getClientByFd(fd);

	// Client is not exist (shouldn't happend)
	if (ptr == NULL)
	{
		return;
	}

	// Do first time thing
	main_obj->notifyErrTooLongOnThisClient(fd);
	ft_utils::logger(ft_utils::DEBUG, "pollin", msg);

	// set isflush toggle
	ptr->setIsFlushing();
}

namespace ft_net
{

void	pollin(FtIrc *main_obj, int fd, int revents)
{
	if (!(revents & POLLIN))
	{
		return;
	}

	try
	{
		// for listen socket
		if (fd == main_obj->getListenFd())
		{
			sb_accept_new_client(main_obj, fd);
			return;
		}

		// for client socket
		sb_read_msg_from_client(main_obj, fd);
	}
	catch (IrcTooLongMsgException const& e)
	{
		sb_handle_too_long(main_obj, fd, e.what());
	}
	catch (IrcInvalidPacketException const& e)
	{
		ft_utils::logger(ft_utils::DEBUG, "pollin", e.what());
	}
	catch (IrcDisconnectedException const&)
	{
		main_obj->notifyQuitOnThisClient(fd, "Connection is closed");
	}
	catch (IrcContinueException const&)
	{
		// Do literal nothing
	}
}

}