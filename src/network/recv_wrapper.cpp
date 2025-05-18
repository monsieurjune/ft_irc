/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recv_wrapper.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:50:21 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/17 20:14:04 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// C Header
#include <sys/types.h>
#include <sys/socket.h>

// Custom Exception
#include "exception/IrcGenericRecvException.hpp"
#include "exception/IrcDisconnectedException.hpp"

namespace ft_net
{

ssize_t recv_wrapper(int fd, void *buff, size_t len, int flags)
{
	ssize_t	recv_len = recv(fd, buff, len, flags);

    // Client is disconnected
	if (recv_len == 0)
	{
		throw IrcDisconnectedException("Client is Disconnected");
	}

    // There should be something to notify when use recv() wrong
	if (recv_len == -1)
	{
		throw IrcGenericRecvException("Encounter -1 when run recv()");
	}

    return recv_len;
}

}
