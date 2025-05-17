/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrcNetwork.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 08:32:28 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/17 09:10:39 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"
#include "std/ft_cstd.hpp"

// C Header
#include <unistd.h>
#include <fcntl.h>

void	FtIrc::addClient(int const fd, const char *ip)
{
	Client*	        ptr = NULL;
    struct pollfd	new_pollfd;

	if (_clientMapByFd.find(fd) != _clientMapByFd.end())
	{
		return;
	}

    // Clear Struct
    ft_std::memset(&new_pollfd, 0, sizeof(new_pollfd));

    // Allocate client
	try
	{
        ptr = new Client(fd);
		ptr->setHost(ip);
	}
	catch (std::bad_alloc const&)
	{
        close(fd);
	}

    // Set NON BLOCKING to fd
    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
    {
        delete ptr;
        return;
    }

    // Set pollfd value
    new_pollfd.fd = fd;
    new_pollfd.events = POLLIN | POLLOUT;
    _mainPollfdVec.push_back(new_pollfd);
    _clientMapByFd[fd] = ptr;
}

void	FtIrc::deleteClient(int const fd)
{
	Client*	ptr = NULL;

	if (_clientMapByFd.find(fd) == _clientMapByFd.end())
	{
		return;
	}

	// Obtain pointer to Client
	ptr = _clientMapByFd[fd];

	// Get object attributes
	std::string const&	nick = ptr->getNickname();

	// Exit from Channel
	for (std::map<std::string, Channel*>::iterator it = _channelMapByName.begin(); it != _channelMapByName.end();)
	{
		if (it->second->hasThisClient(ptr))
		{
			std::map<std::string, Channel*>::iterator	toErase = it;

			it++;
			deleteClientFromChannel(toErase->first, ptr);
		}
		else
		{
			it++;
		}
	}

	// Erase Client from ClientMap (No Free yet)
	_clientMapByNickname.erase(nick);
	_clientMapByFd.erase(fd);

	// Remove fd from pollfd vector by mark it with MARKED_REMOVE_FD
	for (std::vector<struct pollfd>::iterator it = _mainPollfdVec.begin(); it != _mainPollfdVec.end(); it++)
	{
		if (it->fd == fd)
		{
			it->fd = MARKED_REMOVE_FD;
			break;
		}
	}

	// Free Client (close fd here)
	delete ptr;
}

int	FtIrc::createChannel(std::string const& channel_name, Client * const creator)
{
	Channel*	ptr = NULL;

	if (_channelMapByName.find(channel_name) != _channelMapByName.end())
	{
		return (1);
	}

	try
	{
		ptr = new Channel(channel_name, creator);
		_channelMapByName[channel_name] = ptr;

		return (0);
	}
	catch (std::bad_alloc const&)
	{
        delete ptr;
		return (-1);
	}
}

int	FtIrc::deleteClientFromChannel(std::string const& channel_name, Client * const client)
{
	Channel*	ptr = NULL;

	if (_channelMapByName.find(channel_name) == _channelMapByName.end())
	{
		return (1);
	}

    // Get Channel Pointer
	ptr = _channelMapByName[channel_name];

	// Unjoin Client from channel
	if (ptr->deleteUserFromChannel(client) != 0)
	{
		return (2);
	}

	// Check if Channel's members is 0
	if (ptr->getUserCount() == 0)
	{
		_channelMapByName.erase(channel_name);
		delete ptr;
	}
	return (0);
}

int FtIrc::callPoll()
{
    // clean up unused fd everytime
    _tempPollfdVec.clear();

    // Transfer normal fds to new vector
	for (size_t i = 0; i < _mainPollfdVec.size(); i++)
	{
		if (_mainPollfdVec[i].fd != MARKED_REMOVE_FD)
		{
			_tempPollfdVec.push_back(_mainPollfdVec[i]);
		}
	}

    // Swap them back to main vector
	std::swap(_mainPollfdVec, _tempPollfdVec);

    // call poll
    return poll(_mainPollfdVec.data(), _mainPollfdVec.size(), POLL_TIMEOUT_MS);
}