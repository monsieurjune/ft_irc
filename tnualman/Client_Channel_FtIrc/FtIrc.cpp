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

// Hard-coded with fixed array for now, because function pointer's syntax in C++ is harder than I expected...
// Borrowed the idea from the module cpp01/ex05
FtIrc::FtIrc(std::string const password): _serverPassword(password)
{
	// Added _serverName
	_serverName = "ft_irc";
	_commandHandler[0] = &FtIrc::ircCommandKICK;
	_commandHandler[1] = &FtIrc::ircCommandINVITE;
	_commandHandler[2] = &FtIrc::ircCommandMODE;
	_commandHandler[3] = &FtIrc::ircCommandTOPIC;
	_availableCommands = {"KICK", "INVITE", "MODE", "TOPIC"};
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
Client* FtIrc::getClientByNickname(std::string const name) const
{
    try
	{
		return (_clientMapByNickname.at(name));
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

std::string	const & FtIrc::getServerPassword(void) const
{
    return (_serverPassword);
}

/** Returns 0 if client is found and the name is successfully changed;
 * returns -1 if client is not found;
 * returns -2 if the newname alrady exists in the _clientMapByNickname, or if it's the same as the old name.
 */
int FtIrc::changeNickname(std::string const name, std::string const newname)
{
	std::map<std::string, Client*> map = _clientMapByNickname;
	std::map<std::string, Client*>::iterator it = map.find(name);
	
	if (it == map.end())
	{
		std::cerr << "Client with nickname " << name << " not found!" << std::endl;
		return (-1);
	}
	
	if (name == newname || map.find(newname) != map.end())
	{
		std::cerr << "Client with nickname " << newname << " already exists!" << std::endl;
		return (-2);
	}
	
	Client* tmp = it->second;
	map.erase(it);
	map.insert({newname, tmp});

	return (0);
}

int FtIrc::changeChannelName(std::string const name, std::string const newname)
{
	std::map<std::string, Channel*> map = _channelMapByName;
	std::map<std::string, Channel*>::iterator it = map.find(name);
	
	if (it == map.end())
	{
		std::cerr << "Channel named " << name << " not found!" << std::endl;
		return (-1);
	}
	
	if (name == newname || map.find(newname) != map.end())
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

int	FtIrc::addClient(int const fd, std::string const nickname,
						std::string const username, std::string const host, std::string modestr)
{
	if (_clientMapByFd.find(fd) != _clientMapByFd.end())
	{
		std::cerr << "Client with same socket already exists!" << std::endl;
		return (-1);
	}
	
	if (_clientMapByNickname.find(nickname) != _clientMapByNickname.end())
	{
		std::cerr << "Client with same nickname already exists!" << std::endl;
		return (-1);
	}

	Client client(fd, nickname, username, host, modestr);
	
	// Other necessary network and irc operations here
	
	_clientSet.insert(client);
	_clientMapByFd.insert({client.getFd(), &client});
	_clientMapByNickname.insert({client.getNickname(), &client});

	return (0);
}

int	FtIrc::addChannel(std::string const name)
{
	if (_channelMapByName.find(name) != _channelMapByName.end())
	{
		std::cerr << "Channel with same name already exists!" << std::endl;
		return (-1);
	}
	
	Channel channel(name);
	
	// Other necessary network and irc operations here

	_channelSet.insert(channel);
	_channelMapByName.insert({name, &channel});

	return (0);
}

int	FtIrc::deleteClient(Client * const client)
{
	std::set<Client>::iterator it = _clientSet.find(*client);
	
	if (it == _clientSet.end())
	{
		std::cerr << "Invalid client pointer for client deletion!" << std::endl;
		return (-1);
	}
	
	_clientMapByFd.erase(it->getFd());
	_clientMapByNickname.erase(it->getNickname());
	_clientSet.erase(it);

	return (0);
}

int FtIrc::deleteClient(int const fd)
{
	std::map<int, Client*>::iterator it = _clientMapByFd.find(fd);
	
	if (it == _clientMapByFd.end())
	{
		std::cerr << "Client with socket " << fd << " not found!" << std::endl;
		return (-1);
	}
	
	_clientMapByNickname.erase(it->second->getNickname());
	_clientSet.erase(*(it->second));
	_clientMapByFd.erase(fd);

	return (0);
}

int	FtIrc::deleteClient(std::string const nickname)
{
	std::map<std::string, Client*>::iterator it = _clientMapByNickname.find(nickname);
	
	if (it == _clientMapByNickname.end())
	{
		std::cerr << "Client with nickname " << nickname << " not found!" << std::endl;
		return (-1);
	}
	
	_clientSet.erase(*(it->second));
	_clientMapByFd.erase(it->second->getFd());
	_clientMapByNickname.erase(nickname);

	return (0);
}

int FtIrc::deleteChannel(Channel * const channel)
{
	std::set<Channel>::iterator it = _channelSet.find(*channel);

	if (it == _channelSet.end())
	{
		std::cerr << "Invalid channel pointer for client deletion!" << std::endl;
		return (-1);
	}
	
	_channelMapByName.erase(it->getName());
	_channelSet.erase(it);

	return (0);
}

int	FtIrc::deleteChannel(std::string const name)
{
	std::map<std::string, Channel*>::iterator it = _channelMapByName.find(name);

	if (it == _channelMapByName.end())
	{
		std::cerr << "Channel named " << name << " not found!" << std::endl;
		return (-1);
	}
	
	_channelSet.erase(*(it->second));
	_channelMapByName.erase(name);

	return (0);
}
