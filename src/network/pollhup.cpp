/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pollhup.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:52:52 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/17 21:00:12 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "network/network.hpp"
#include "utils/ft_utils.hpp"

namespace ft_net
{

void	pollhup(FtIrc *main_obj, int fd, int revents)
{
    if (!(revents & POLLHUP))
    {
        return;
    }

    main_obj->notifyQuitOnThisClient(fd, "Connection is closed");
}

}