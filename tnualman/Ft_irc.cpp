/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ft_irc.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 01:58:07 by tnualman          #+#    #+#             */
/*   Updated: 2024/12/29 02:59:57 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ft_irc.hpp"

Ft_irc::Ft_irc(void)
{
    std::time(&_timeServerStarted);
}

Ft_irc::Ft_irc(std::string const password): _serverPassword(password)
{
    std::time(&_timeServerStarted);
}

Ft_irc::~Ft_irc(void) {}

Client* Ft_irc::getClientByFd(int const fd) const
{
    try
	{
		return (_clientMapByFd.at(fd));
	}
	catch (std::exception const & e)
	{
		return (NULL);
	}	
}
Client* Ft_irc::getClientByUsername(std::string const name) const
{
    try
	{
		return (_clientMapByUsername.at(name));
	}
	catch (std::exception const & e)
	{
		return (NULL);
	}	
}

Channel* Ft_irc::getChannelByName(std::string const name) const
{
    try
	{
		return (_channelMapByName.at(name));
	}
	catch (std::exception const & e)
	{
		return (NULL);
	}	
}

std::string	Ft_irc::getServerPassword(void) const
{
    return (_serverPassword);
}