/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:13:07 by tponutha          #+#    #+#             */
/*   Updated: 2025/01/30 20:13:07 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __NETWORK_HPP__
#define __NETWORK_HPP__

// Project Header
#include "ft_irc/FtIrcConstants.hpp"

// CPP Header
#include <string>

namespace ft_net
{

int		get_listener_scoket_fd(const char *port_str);
// bool	irc_recv(int fd, std::string& msg);
std::string	irc_recv(int fd);

}

#endif