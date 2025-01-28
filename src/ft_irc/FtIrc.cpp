/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 17:26:36 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/28 12:47:04 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrc.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"
#include "ft_irc/Message.hpp"

// C Header
#include <unistd.h>

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
		return 1;
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
		ptr = new Client(fd);
		_clientMapByFd[fd] = ptr;
	}
	catch (std::bad_alloc const& e)
	{
		if (ptr != NULL)
		{
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
	std::string const&			nick = ptr->getNickname();

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

	delete ptr;	// Free Client
}