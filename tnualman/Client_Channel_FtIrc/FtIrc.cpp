/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 01:58:07 by tnualman          #+#    #+#             */
/*   Updated: 2024/12/31 20:50:08 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FtIrc.hpp"

FtIrc::FtIrc(void)
{
    std::time(&_timeServerStarted);
}

FtIrc::FtIrc(std::string const password): _serverPassword(password)
{
    std::time(&_timeServerStarted);
}

FtIrc::~FtIrc(void) {}

Client* FtIrc::getClientByFd(int const fd) const
{
    try
	{
		return (_clientMapByFd.at(fd));
	}
	catch (std::exception const & e)
	{
		std::cerr << "Client with socket " << fd << " not found!" << std::endl;
		return (NULL);
	}	
}
Client* FtIrc::getClientByUsername(std::string const name) const
{
    try
	{
		return (_clientMapByUsername.at(name));
	}
	catch (std::exception const & e)
	{
		std::cerr << "Client with username " << name << " not found!" << std::endl;
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
		std::cerr << "Channel named " << name << " not found!" << std::endl;
		return (NULL);
	}	
}

std::string	FtIrc::getServerPassword(void) const
{
    return (_serverPassword);
}

/** Returns 0 if client is found and the name is successfully changed;
 * returns -1 if client is not found;
 * returns -2 if the newname alrady exists in the _clientMapByUsername, or if it's the same as the old name.
 */
int FtIrc::changeUsername(std::string const name, std::string newname)
{
	std::map<std::string, Client*> map = _clientMapByUsername;
	std::map<std::string, Client*>::iterator it = map.find(name);
	
	if (it == end(map))
	{
		std::cerr << "Client with username " << name << " not found!" << std::endl;
		return (-1);
	}
	
	if (name == newname || map.find(newname) != end(map))
	{
		std::cerr << "Client with username " << newname << " already exists!" << std::endl;
		return (-2);
	}
	
	Client* tmp = it->second;
	map.erase(it);
	map.insert({newname, tmp});

	return (0);
}

int FtIrc::changeChannelName(std::string const name, std::string newname)
{
	std::map<std::string, Channel*> map = _channelMapByName;
	std::map<std::string, Channel*>::iterator it = map.find(name);
	
	if (it == end(map))
	{
		std::cerr << "Channel named " << name << " not found!" << std::endl;
		return (-1);
	}
	
	if (name == newname || map.find(newname) != end(map))
	{
		std::cerr << "Channel named " << newname << " already exists!" << std::endl;
		return (-2);
	}
	
	Channel* tmp = it->second;
	map.erase(it);
	map.insert({newname, tmp});

	return (0);
}

void FtIrc::changeServerPassword(std::string password)
{
	_serverPassword = password;
}

int	FtIrc::addClient(Client * const client)
{
	if (_clientMapByFd.find(client->getFd()) != end(_clientMapByFd))
	{
		std::cerr << "Client with same socket already exists!" << std::endl;
		return (-1);
	}
	
	// Other necessary network and irc operations here

	_clientMapByFd.insert({client->getFd(), client});
	_clientMapByUsername.insert({client->getUsername(), client});

	return (0);
}

int	FtIrc::addChannel(Channel * const channel)
{
	if (_channelMapByName.find(channel->getName()) != end(_channelMapByName))
	{
		std::cerr << "Channel with same name already exists!" << std::endl;
		return (-1);
	}
	
	// Other necessary network and irc operations here

	_channelMapByName.insert({channel->getName(), channel});

	return (0);
}
