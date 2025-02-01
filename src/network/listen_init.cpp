/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listen_init.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:13:36 by tponutha          #+#    #+#             */
/*   Updated: 2025/01/30 20:13:36 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/network.hpp"
#include "std/ft_cstd.hpp"
#include "std/ft_cppstd.hpp"

#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

static ssize_t	sb_check_port_str(const char *num)
{
	ssize_t	port_no;

	if (!ft_std::isnumber(num))
	{
		// TODO: Put log here
		return -1;
	}

	port_no = ft_std::stoi(num);
	if (port_no < 0 || port_no > 65535)
	{
		// TODO: Put log here
		return -1;
	}
	return port_no;
}

static bool sb_get_addr_info(const char *port_no, struct addrinfo **pai)
{
	struct addrinfo hints;
	int				res;

	ft_std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	res = getaddrinfo(NULL, port_no, &hints, pai);
	if (res != 0)
	{
		// TODO: Put log here
		return false;
	}
	return true;
}

static void	sb_setsockopt_helper(int listener_sockfd)
{
	int	reuse_addr = 1;	// true

	// Set Socket to be reuse quickly after restart Program
	if (setsockopt(listener_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr)) != 0)
	{
		// TODO: Put log here
		return;
	}
}

namespace ft_net
{

int	get_listener_scoket_fd(const char *port_str)
{
	ssize_t	port_no					= sb_check_port_str(port_str);
	struct addrinfo *ai 			= NULL;
	struct addrinfo *node 			= NULL;
	int				listen_socketfd	= 0;

	// Check if port_no is valid
	if (port_no < 0)
	{
		return -1;
	}

	// Check if getaddrinfo is success
	if (!sb_get_addr_info(port_str, &ai))
	{
		return -1;
	}

	// Iterate over ai's nodes
	// if fail within loop then change listen socket, and do it again
	// until either bind success or iterate whole loop
	for (node = ai; node != NULL; node = node->ai_next)
	{
		// Try to allocate listen socket
		listen_socketfd = socket(node->ai_family, node->ai_socktype, node->ai_protocol);
		if (listen_socketfd < 0)
		{
			// TODO: Put log here
			continue;
		}
		
		// Set Option to listen socket
		sb_setsockopt_helper(listen_socketfd);

		// Bind this listen socket
		if (bind(listen_socketfd, node->ai_addr, node->ai_addrlen) == 0)
		{
			// TODO: Put log here
			break;
		}

		// TODO: Put log here
		close(listen_socketfd);
	}
	freeaddrinfo(ai);

	// It shouldn't iterate whole struct
	if (node == NULL)
	{
		// TODO: Put log here
		return -1;
	}

	// Set listen socket to Non-Blocking
	if (fcntl(listen_socketfd, F_SETFL, O_NONBLOCK) < 0)
	{
		// TODO: Put log here
		close(listen_socketfd);
		return -1;
	}

	// Set this socket to listen now
	if (listen(listen_socketfd, 1024) < 0)
	{
		// TODO: Put log here
		close(listen_socketfd);
		return -1;
	}
	return listen_socketfd;
}

}