/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listen_init.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:13:36 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/17 03:36:28 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "network/network.hpp"
#include "std/ft_cstd.hpp"
#include "std/ft_cppstd.hpp"
#include "utils/ft_utils.hpp"

// Custom Exception
#include "exception/IrcListenBindingException.hpp"

// C Header (fd)
#include <unistd.h>
#include <fcntl.h>

// C Header (Network)
#include <sys/types.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

// marcro
#define LOCAL_LOG_NAME "listen_init"

static inline void	sb_check_port_str(const char *num)
{
	ssize_t	port_no;

	if (!ft_std::isnumber(num))
	{
		std::string	msg = std::string("Port <") + std::string(num) + std::string("> isn\'t number");
		throw IrcListenBindingException(msg);
	}

	port_no = ft_std::stoi(num);
	if (port_no < 0 || port_no > 65535)
	{
		std::string	msg = std::string("Port <") + std::string(num) + std::string("> isn\'t in [0, 65535] range");
		throw IrcListenBindingException(msg);
	}

	if (port_no >= 0 && port_no <= 1023)
	{
		ft_utils::logger(ft_utils::WARN, LOCAL_LOG_NAME, "Trying to use Well-Known ports [0, 1023]");
	}
}

static inline void sb_get_addr_info(const char *port_no, struct addrinfo **pai)
{
	struct addrinfo hints;

	ft_std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;					// IPv4
	hints.ai_socktype = SOCK_STREAM;			// TCP
	hints.ai_flags = AI_PASSIVE;				// Listen to anyone

	if (getaddrinfo(NULL, port_no, &hints, pai) != 0)
	{
		throw IrcListenBindingException("Can\'t get this port\'s addresss");
	}
}

static inline void	sb_setsockopt_helper(int listener_sockfd)
{
	int	reuse_addr = 1;	// true

	// Set Socket to be reuse quickly after restart Program
	if (setsockopt(listener_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr)) != 0)
	{
		ft_utils::logger(ft_utils::WARN, LOCAL_LOG_NAME, "Error occur when trying to set socket\'s option");
		return;
	}
}

static inline int	sb_binding(struct addrinfo **pai)
{
	int				listen_socketfd;
	struct addrinfo *node	= NULL;

	// Iterate over ai's nodes
	// if fail within loop then change listen socket, and do it again
	// until either bind success or iterate whole loop
	for (node = *pai; node != NULL; node = node->ai_next)
	{
		// Try to allocate listen socket
		listen_socketfd = socket(node->ai_family, node->ai_socktype, node->ai_protocol);
		if (listen_socketfd < 0)
		{
			continue;
		}

		// Set Option to listen socket
		sb_setsockopt_helper(listen_socketfd);

		// Bind this listen socket
		if (bind(listen_socketfd, node->ai_addr, node->ai_addrlen) == 0)
		{
			break;
		}

		// Close, if binding is failed
		close(listen_socketfd);
	}

	// free ai
	freeaddrinfo(*pai);
	*pai = NULL;

	// If it iterated whole list, then there is no suitable socket
	if (node == NULL)
	{
		throw IrcListenBindingException("Binding socket is failed");
	}

	return listen_socketfd;
}

namespace ft_net
{

int	get_listener_scoket_fd(const char *port_str)
{
	struct addrinfo*	ai				= NULL;
	int					listen_socketfd	= 0;

	try
	{
		// Check if port_no is valid
		sb_check_port_str(port_str);

		// Check if getaddrinfo is success
		sb_get_addr_info(port_str, &ai);

		// Binding Socket
		listen_socketfd = sb_binding(&ai);

		// Set listen socket to Non-Blocking
		if (fcntl(listen_socketfd, F_SETFL, O_NONBLOCK) < 0)
		{
			close(listen_socketfd);
			throw IrcListenBindingException("Set socket to non-blocking is failed");
		}

		// Set this socket to listen now
		if (listen(listen_socketfd, 1024) < 0)
		{
			close(listen_socketfd);
			throw IrcListenBindingException("Set listen() to socket is failed");
		}
		ft_utils::logger(ft_utils::INFO, LOCAL_LOG_NAME, "Binding listener socket is succesful");

		return listen_socketfd;
	}
	catch (IrcListenBindingException const& e)
	{
		ft_utils::logger(ft_utils::CRITICAL, LOCAL_LOG_NAME, e.what());

		return -1;
	}
}

}