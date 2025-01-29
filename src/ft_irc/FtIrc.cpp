/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 17:26:36 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/30 01:57:55 by tponutha         ###   ########.fr       */
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


FtIrc::FtIrc(std::string const name, std::string const password) : _serverName(name), \
																	_serverPassword(password), \
																	_timeServerStarted(std::time(NULL))
{
	// NO PASSWORD CMD

	// AUTHEN CMD

	// NORMAL CMD
}

FtIrc::~FtIrc()
{
	// Free Client
	for (std::map<int, Client*>::iterator it = _clientMapByFd.begin(); it != _clientMapByFd.end(); it++)
	{
		delete it->second;
		it->second = NULL;
	}

	// Free Channel
	for (std::map<std::string, Channel*>::iterator it = _channelMapByName.begin(); it != _channelMapByName.end(); it++)
	{
		delete it->second;
		it->second = NULL;
	}
}

Client*	FtIrc::getClientByFd(int const fd) const
{
    try
	{
		return (_clientMapByFd.at(fd));
	}
	catch (std::exception const & e)
	{
		// std::cerr << "Client with socket " << fd << " not found!" << std::endl;
		return (NULL);
	}	
}

Client*	FtIrc::getClientByNickname(std::string const name) const
{
    try
	{
		return (_clientMapByNickname.at(name));
	}
	catch (std::exception const & e)
	{
		// std::cerr << "Client with username " << name << " not found!" << std::endl;
		return (NULL);
	}	
}

Channel* FtIrc::getChannelByName(std::string const name) const
{
    try
	{
		return (_channelMapByName.at(name));
	}
	catch (std::exception const & e)
	{
		// std::cerr << "Channel named " << name << " not found!" << std::endl;
		return (NULL);
	}	
}

const struct pollfd*	FtIrc::getPollFd() const
{
	return (_pollfdVec.data());
}

int	FtIrc::createChannel(std::string const channel_name, Client * const creator)
{
	Channel*	ptr = NULL;
	
	if (_channelMapByName.find(channel_name) != _channelMapByName.end())
	{
		// TODO: Handle this
		return 1;
	}

	try
	{
		ptr = new Channel(channel_name, creator);
		_channelMapByName[channel_name] = ptr;
		
		return 0;
	}
	catch (std::bad_alloc const& e)
	{
		if (ptr != NULL)
		{
			delete ptr;
		}
		return -1;
	}
}

int	FtIrc::deleteClientFromChannel(std::string const channel_name, Client * const client)
{
	Channel*	ptr = NULL;

	if (_channelMapByName.find(channel_name) == _channelMapByName.end())
	{
		// TODO: Handle this
		return 1;
	}

	ptr = _channelMapByName[channel_name];

	// Unjoin Client from channel
	if (ptr->deleteUserFromChannel(client) != 0)
	{
		return 2;
	}

	// Check if Channel's members is 0
	if (ptr->getUserCount() == 0)
	{
		_channelMapByName.erase(channel_name);
		delete ptr;
	}
	return 0;
}

void	FtIrc::addClient(int const fd)
{
	Client*	ptr = NULL;

	if (_clientMapByFd.find(fd) != _clientMapByFd.end())
	{
		return;
	}

	// Other necessary network and irc operations here
	try
	{
		struct pollfd	new_pollfd;

		ft_std::memset(&new_pollfd, 0, sizeof(new_pollfd));
		ptr = new Client(fd);

		// Set NON-BLOCKING
		if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		{
			delete ptr;
			return;
		}

		// Set pollfd value
		new_pollfd.fd = fd;
		new_pollfd.events = POLLIN | POLLOUT;

		_pollfdVec.push_back(new_pollfd);
		_clientMapByFd[fd] = ptr;
	}
	catch (std::bad_alloc const& e)
	{
		if (ptr != NULL)
		{
			_pollfdVec.pop_back();
			delete ptr;
		}
		else
		{
			close(fd);
		}
	}
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

	// Remove fd from pollfd vector
	for (std::vector<struct pollfd>::iterator it = _pollfdVec.begin(); it != _pollfdVec.end(); it++)
	{
		if (it->fd == fd)
		{
			_pollfdVec.erase(it);
			break;
		}
	}

	// Erase Client from ClientMap (No Free yet)
	_clientMapByNickname.erase(nick);
	_clientMapByFd.erase(fd);

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

	// Free Client
	delete ptr;
}