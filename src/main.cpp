/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:13:54 by tponutha          #+#    #+#             */
/*   Updated: 2025/01/31 18:11:34 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/network.hpp"
#include "utils/ft_utils.hpp"
#include "ft_irc/FtIrc.hpp"
#include "exception/IrcDisconnectedException.hpp"

bool	g_run	= true;

static void	sb_killer(int signum)
{
	(void)signum;
	g_run = false;
}

static void	sb_ignorer(int signum)
{
	(void)signum;	// ignore
}

static inline int	sb_initializer(const int argc, const char *argv[])
{
	if (argc != 3)
	{
		// TODO: Fix log here
		return -1;
	}

	if (argv[2][0] == '\0')
	{
		// TODO: Fix log here
		return -1;
	}

	if (ft_utils::signal_init(sb_killer, sb_ignorer) != 0)
	{
		// TODO: Fix log here
		return -1;
	}

	return ft_net::get_listener_scoket_fd(argv[1]);
}

#include <iostream>

static inline void	sb_poll(FtIrc *main_obj)
{
	std::vector<struct pollfd>&	pollfd_vec = main_obj->getPollFdVector();
	int	poll_count;

	poll_count = poll(pollfd_vec.data(), pollfd_vec.size(), POLL_TIMEOUT_MS);
	if (poll_count < 0)
	{
		return;
	}

	for (size_t i = 0; i < pollfd_vec.size(); i++)
	{
		int	fd		= pollfd_vec[i].fd;
		int	revents	= pollfd_vec[i].revents;

		try
		{
			ft_net::pollin(main_obj, fd, revents);
			ft_net::pollout(main_obj, fd, revents);
		}
		catch (const IrcDisconnectedException& e)
		{
			std::cout << "Client Disconnected: " << main_obj->getClientByFd(fd)->getHost() << std::endl;
			main_obj->deleteClient(fd);
		}
		catch (const std::exception& e)
		{
			// Ignored
		}
	}

	// Clean any closed fd from pollfd
	main_obj->cleanUnusedPollFd();
}

int	main(const int argc, const char *argv[])
{
	int	listen_socketfd;

	listen_socketfd = sb_initializer(argc, argv);
	if (listen_socketfd == -1)
	{
		return 1;
	}

	FtIrc	main_obj(argv[2], "irc.localhost.test", listen_socketfd);

	// Infinite Loop
	while (g_run)
	{
		try
		{
			sb_poll(&main_obj);
		}
		catch (const std::exception& e)
		{
			continue;
		}
	}

	return 0;
}