/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pollin.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:52:55 by tponutha          #+#    #+#             */
/*   Updated: 2025/02/01 14:04:33 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "network/network.hpp"
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Message.hpp"

#include "exception/IrcContinueException.hpp"
#include "exception/IrcInvalidPacketException.hpp"
#include "exception/IrcTooLongMsgException.hpp"

// C Header
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// CPP Header
#include <iostream>

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

	std::cout << "New Client: " << client_ip << std::endl;
}

static inline void	sb_read_msg_from_client(FtIrc *main_obj, int fd)
{
	std::string	raw_msg = ft_net::irc_recv(fd);
	Message	msg(raw_msg);
	Client	*client = main_obj->getClientByFd(fd);

	// Client is not exist (shouldn't happen)
	if (client == NULL)
	{
		return;
	}

	if (!msg.isValid())
	{
		return;
	}

	main_obj->ircMessageHandler(msg, client);
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
		if (fd == main_obj->getListenFd())
		{
			sb_accept_new_client(main_obj, fd);
		}
		else
		{
			sb_read_msg_from_client(main_obj, fd);
		}
	}
	catch (const IrcTooLongMsgException& e)
	{
		// TODO: later
	}
	catch (const IrcInvalidPacketException& e)
	{
		// Ignored
	}
	catch (const IrcContinueException& e)
	{
		// Ignored
	}
}

}