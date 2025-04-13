/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 17:26:36 by tnualman          #+#    #+#             */
/*   Updated: 2025/04/13 09:40:16 by tponutha         ###   ########.fr       */
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

FtIrc::FtIrc(std::string const password, std::string const servername, int const listen_fd) 
		: _listen_fd(listen_fd), \
		_serverName(servername), \
		_serverPassword(password), \
		_timeServerStarted(std::time(NULL))
{
	// Add listen fd to poll fd
	struct pollfd	new_pollfd;

	ft_std::memset(&new_pollfd, 0, sizeof(new_pollfd));
	new_pollfd.fd = listen_fd;
	new_pollfd.events = POLLIN;
	_mainPollfdVec.push_back(new_pollfd);

	// NO PASSWORD CMD
	_noPassCmdMap["PASS"] = ircPASS;
	_noPassCmdMap["CAP"] = ircCAP;
	_noPassCmdMap["PING"] = ircPING;
	_noPassCmdMap["PONG"] = ircPONG;
	_noPassCmdMap["QUIT"] = ircQUIT;

	// AUTHEN CMD
	_authenCmdMap["NICK"] = ircNICK;
	_authenCmdMap["USER"] = ircUSER;

	// NORMAL CMD
	_normalCmdMap["JOIN"] = ircJOIN;
	_normalCmdMap["INVITE"] = ircINVITE;
	_normalCmdMap["KICK"] = ircKICK;
	_normalCmdMap["MODE"] = ircMODE;
	_normalCmdMap["TOPIC"] = ircTOPIC;
	_normalCmdMap["PRIVMSG"] = ircPRIVMSG;
	_normalCmdMap["PART"] = ircPART;
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

	// Close Listen fd
	close(_listen_fd);
}

int	FtIrc::getListenFd()	const
{
	return (_listen_fd);
}

std::string const&	FtIrc::getServerName()	const
{
	return (_serverName);
}

std::string const&	FtIrc::getServerPassword()	const
{
	return (_serverPassword);
}

std::vector<struct pollfd>&	FtIrc::getPollFdVector()
{
	return (_mainPollfdVec);
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

std::set<Channel*>	FtIrc::getChannelSetByClient(Client * const client)
{
	std::set<Channel*>							channelSet;
	std::map<std::string, Channel*>::iterator	it;

	for (it = _channelMapByName.begin(); it != _channelMapByName.end(); it++)
	{
		if (it->second->hasThisClient(client))
		{
			channelSet.insert(it->second);
		}
	}

	return channelSet;
}

void	FtIrc::changeClientNickname(std::string const& old_nick, std::string const& new_nick)
{
	std::map<std::string, Client*>::iterator	it_new = _clientMapByNickname.find(new_nick);
	std::map<std::string, Client*>::iterator	it_old = _clientMapByNickname.find(old_nick);

	// true, if new_nick is already existed
	if (it_new != _clientMapByNickname.end())
	{
		return;
	}

	// true, if old_nick isn't existed
	if (it_old == _clientMapByNickname.end())
	{
		return;
	}

	Client*	ptr = it_old->second;

	ptr->setNickname(new_nick);
	_clientMapByNickname.erase(it_old);
	_clientMapByNickname[new_nick] = ptr;
}

void	FtIrc::setClientNickname(Client * const client, std::string const& nick)
{
	std::map<std::string, Client*>::iterator	it1 = _clientMapByNickname.find(nick);

	// true, if nick is already existed
	if (it1 != _clientMapByNickname.end())
	{
		return;
	}

	client->setNickname(nick);
	_clientMapByNickname[nick] = client;
}

void	FtIrc::cleanUnusedPollFd()
{
	_tempPollfdVec.clear();

	for (size_t i = 0; i < _mainPollfdVec.size(); i++)
	{
		if (_mainPollfdVec[i].fd != MARKED_REMOVE_FD)
		{
			_tempPollfdVec.push_back(_mainPollfdVec[i]);
		}
	}
	std::swap(_mainPollfdVec, _tempPollfdVec);
}

int	FtIrc::createChannel(std::string const channel_name, Client * const creator)
{
	Channel*	ptr = NULL;
	
	if (_channelMapByName.find(channel_name) != _channelMapByName.end())
	{
		// TODO: Handle this
		return (1);
	}

	try
	{
		ptr = new Channel(channel_name, creator);
		_channelMapByName[channel_name] = ptr;
		
		return (0);
	}
	catch (std::bad_alloc const& e)
	{
		if (ptr != NULL)
		{
			delete ptr;
		}
		return (-1);
	}
}

int	FtIrc::deleteClientFromChannel(std::string const channel_name, Client * const client)
{
	Channel*	ptr = NULL;

	if (_channelMapByName.find(channel_name) == _channelMapByName.end())
	{
		// TODO: Handle this
		return (1);
	}

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

void	FtIrc::addClient(int const fd, const char *ip)
{
	Client*	ptr = NULL;

	if (_clientMapByFd.find(fd) != _clientMapByFd.end())
	{
		return;
	}

	try
	{
		struct pollfd	new_pollfd;

		ft_std::memset(&new_pollfd, 0, sizeof(new_pollfd));
		ptr = new Client(fd);
		ptr->setHost(ip);

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
	catch (std::bad_alloc const& e)
	{
		if (ptr != NULL)
		{
			_mainPollfdVec.pop_back();
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

	// Remove fd from pollfd vector
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

void	FtIrc::applyReplyBatchToClient(t_replyBatch& batch)
{
	for (std::vector<t_reply>::iterator it = batch.begin(); it != batch.end(); it++)
	{
		Client					*client	= it->first;
		std::queue<Message>&	queue	= it->second;

		while (!queue.empty())
		{
			std::string	msg	= queue.front().assembleRawMessage();

			client->enqueueReply(msg);
			queue.pop();
		}
	}
}

void	FtIrc::ircMessageHandler(Message const & msg, Client * const client)
{
	std::string const&	cmd	= msg.getCommand();

	if (_noPassCmdMap.find(cmd) != _noPassCmdMap.end())
	{
		t_IrcCmd		caller	= _noPassCmdMap[cmd];
		t_replyBatch	batch	= caller(this, msg, client);

		applyReplyBatchToClient(batch);
		return;
	}

	if (_authenCmdMap.find(cmd) != _authenCmdMap.end())
	{
		if (client->containFlags(PASS_FLAG) || client->containFlags(DEBUG_FLAG))
		{
			t_IrcCmd		caller	= _authenCmdMap[cmd];
			t_replyBatch	batch	= caller(this, msg, client);

			applyReplyBatchToClient(batch);
			return;
		}

		//  TODO: Return Not Registered
		return;
	}

	if (_normalCmdMap.find(cmd) != _normalCmdMap.end())
	{
		if (client->containFlags(PASS_FLAG | NICK_FLAG | USER_FLAG) || client->containFlags(DEBUG_FLAG))
		{
			t_IrcCmd		caller	= _normalCmdMap[cmd];
			t_replyBatch	batch	= caller(this, msg, client);

			applyReplyBatchToClient(batch);
			return;
		}

		//  TODO: Return Not Registered
		return;
	}

	// TODO: Return Unknow Command
}


