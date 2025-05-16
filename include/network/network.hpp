/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:13:07 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/17 03:44:19 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __NETWORK_HPP__
#define __NETWORK_HPP__

// Project Header
#include "ft_irc/FtIrcConstants.hpp"

// CPP Header
#include <string>

#ifndef LISTEN_QUEUE_SIZE
#define LISTEN_QUEUE_SIZE 128
#endif

#ifndef POLL_TIMEOUT_MS
#define POLL_TIMEOUT_MS 100
#endif

// Forward Declaration to avoid include too much dependencies
class FtIrc;

namespace ft_net
{

/**
 * @brief Initialize listener socket
 * 
 * @return Listener Socket fd
 * 
 * - -1: failed
 * 
 * - >0: succesful
 */
int	get_listener_scoket_fd(const char *port_str);

/**
 * @brief Recieve Function that comply with IRC Rule
 * 
 * @param fd socket's fd
 * 
 * @return Message String as rvalue
 * 
 * @throws IrcContinueException: when there is no terminate bytes yet
 * @throws IrcTooLongException: when message is exceed 512 bytes
 * @throws IrcInvalidPacketException: when message contain CR, LF, NUL
 * @throws IrcDisconnectedException: when socket is closed from client side
 * @throws CstdException: when recv() triggered c's errno (i.e. ECONNRESET)
 */
std::string	irc_recv(int fd);

/**
 * @brief Handler for POLLIN (ready to read)
 * 
 * @param main_obj Pointer to FtIrc Class
 * @param fd Client's socket fd
 * @param revent Reevent Flags, obtained from poll()
 * 
 * @throws IrcDisconnectedException: when socket is closed from client side
 */
void	pollin(FtIrc *main_obj, int fd, int revents);

/**
 * @brief Handler for POLLOUT (ready to write)
 * 
 * @param main_obj Pointer to FtIrc Class
 * @param fd Client's socket fd
 * @param revent Reevent Flags, obtained from poll()
 * 
 * @throws IrcDisconnectedException: when socket is closed from client side
 */
void	pollout(FtIrc *main_obj, int fd, int revents);

/**
 * @brief Handler for POLLHUP (disconnected)
 * 
 * @param main_obj Pointer to FtIrc Class
 * @param fd Client's socket fd
 * @param revent Reevent Flags, obtained from poll()
 * 
 * @throws IrcDisconnectedException: when socket is closed from client side
 */
void	pollhup(FtIrc *main_obj, int fd, int revents);

}

#endif