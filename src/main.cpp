/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:13:54 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/18 06:46:18 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project header
#include "ft_irc/FtIrc.hpp"
#include "network/network.hpp"
#include "utils/ft_utils.hpp"

// Use for check running
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

static inline void	sb_print_attribute()
{
	std::string	msg;

	msg.append("DEBUG = ");
	msg.append(DEBUG_MODE ? "TRUE" : "FALSE");
	ft_utils::logger(ft_utils::INFO, "main", msg);
	msg.clear();

	msg.append("VERSION = ");
	msg.append(FT_IRC_VERSION);
	ft_utils::logger(ft_utils::INFO, "main", msg);
	msg.clear();

	msg.append("SERVER = irc.localhost.test");
	ft_utils::logger(ft_utils::INFO, "main", msg);
	msg.clear();

	msg.append("NETWORK = FtIrcNetwork");
	ft_utils::logger(ft_utils::INFO, "main", msg);
	msg.clear();
}

static inline bool	sb_check_argv(const int argc, const char *argv[])
{
	bool	ret = true;

	if (argc != 3)
	{
		ft_utils::logger(ft_utils::CRITICAL, "main", "Arguments must have exactly 2 params");
		return false;
	}

	if (argv[1][0] == '\0')
	{
		ft_utils::logger(ft_utils::CRITICAL, "main", "Port musn\'t empty");
		ret = false;
	}

	if (argv[2][0] == '\0')
	{
		ft_utils::logger(ft_utils::CRITICAL, "main", "Password musn\'t empty");
		ret = false;
	}

	return ret;
}

static inline void	sb_network_loop(FtIrc *main_obj)
{
	int							poll_count = main_obj->callPoll();
	std::vector<struct pollfd>&	pollfd_vec = main_obj->getPollFdVector();

	if (poll_count < 0)
	{
		ft_utils::logger(ft_utils::ERROR, "main", "Encounter error after ran poll()");
		return;
	}

	for (size_t i = 0; i < pollfd_vec.size(); i++)
	{
		int	fd		= pollfd_vec[i].fd;
		int	revents	= pollfd_vec[i].revents;

		try
		{
			ft_net::pollhup(main_obj, fd, revents);
			ft_net::pollin(main_obj, fd, revents);
			ft_net::pollout(main_obj, fd, revents);
		}
		catch (std::exception const& e)
		{
			ft_utils::logger(ft_utils::ERROR, "main", e.what());
		}
	}
}

static inline void	sb_server(int listener_socket_fd, const char* password)
{
	FtIrc	main_obj(
				listener_socket_fd, 
				"irc.localhost.test",
				"FtIrcNetwork",
				password
			);

	// Run
	while (g_run)
	{
		try
		{
			sb_network_loop(&main_obj);
		}
		catch (std::exception const& e)
		{
			ft_utils::logger(ft_utils::ERROR, "main", e.what());
			continue;
		}
	}
}

int	main(const int argc, const char *argv[])
{
	// show
	sb_print_attribute();

	// actual logic
	try
	{
		int	listener_socket_fd = 0;

		// Check argv
		if (!sb_check_argv(argc, argv))
		{
			return 1;
		}

		// Init signal handlers
		if (ft_utils::signal_init(sb_killer, sb_ignorer))
		{
			return 1;
		}

		// Binding socket
		listener_socket_fd = ft_net::get_listener_scoket_fd(argv[1]);
		if (listener_socket_fd < 0)
		{
			return 1;
		}

		// Run Server
		sb_server(listener_socket_fd, argv[2]);
	}
	catch (std::exception const& e)
	{
		ft_utils::logger(ft_utils::CRITICAL, "main", e.what());
	}

	return 0;
}