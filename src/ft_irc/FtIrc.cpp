/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 17:26:36 by tnualman          #+#    #+#             */
/*   Updated: 2025/05/17 09:05:48 by tponutha         ###   ########.fr       */
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

FtIrc::FtIrc(
		int const listen_fd, 
		std::string const& servername,
		std::string const& networkname,
		std::string const& password
	)
		: _listen_fd(listen_fd), \
		_serverName(servername), \
		_networkName(networkname), \
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
	}

	// Free Channel
	for (std::map<std::string, Channel*>::iterator it = _channelMapByName.begin(); it != _channelMapByName.end(); it++)
	{
		delete it->second;
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

std::string const&	FtIrc::getNetworkName() const
{
	return (_networkName);
}

std::string const&	FtIrc::getServerPassword()	const
{
	return (_serverPassword);
}

std::time_t const&	FtIrc::getTimeServerCreated() const
{
	return (_timeServerStarted);
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
	catch (std::exception const&)
	{
		return (NULL);
	}	
}

Client*	FtIrc::getClientByNickname(std::string const& name) const
{
    try
	{
		return (_clientMapByNickname.at(name));
	}
	catch (std::exception const&)
	{
		return (NULL);
	}	
}

Channel* FtIrc::getChannelByName(std::string const& name) const
{
    try
	{
		return (_channelMapByName.at(name));
	}
	catch (std::exception const&)
	{
		return (NULL);
	}	
}

std::set<Channel*>	FtIrc::getChannelSetByClient(Client * const client) const
{
	std::set<Channel*>								channelSet;
	std::map<std::string, Channel*>::const_iterator	it;

	for (it = _channelMapByName.begin(); it != _channelMapByName.end(); it++)
	{
		if (it->second->hasThisClient(client))
		{
			channelSet.insert(it->second);
		}
	}

	return channelSet;
}

std::set<Client*>	FtIrc::getClientSetByChannel(Channel * const channel) const
{
	std::set<Client*>					clientSet;
	Channel::t_userMap const&			userMap = channel->getUserMap();
	Channel::t_userMap::const_iterator	it;

	for (it = userMap.begin(); it != userMap.end(); it++)
	{
		clientSet.insert(it->first);
	}

	return clientSet;
}

std::set<Client*>	FtIrc::getClientSetByChannelSet(std::set<Channel*> const& channelSet) const
{
	std::set<Client*>					clientSet;
	std::set<Channel*>::const_iterator	it;

	for (it = channelSet.begin(); it != channelSet.end(); it++)
	{
		std::set<Client*>	clientSubSet = getClientSetByChannel(*it);

		clientSet.insert(clientSubSet.begin(), clientSubSet.end());
	}

	return clientSet;
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

	// replace old nickname with new one
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

	// NORMAL CMD
	if (_normalCmdMap.find(cmd) != _normalCmdMap.end())
	{
		if (client->containFlags(LOGIN_FLAG) || client->containFlags(DEBUG_FLAG))
		{
			t_IrcCmd		caller	= _normalCmdMap[cmd];
			t_replyBatch	batch	= caller(this, msg, client);

			applyReplyBatchToClient(batch);
			return;
		}

		// Return Not Registered
		t_replyBatch	batch = errNotRegistered(client);

		applyReplyBatchToClient(batch);
		return;
	}

	// NO PASS CMD
	if (_noPassCmdMap.find(cmd) != _noPassCmdMap.end())
	{
		t_IrcCmd		caller	= _noPassCmdMap[cmd];
		t_replyBatch	batch	= caller(this, msg, client);

		applyReplyBatchToClient(batch);
		return;
	}

	// AUTHEN CMD
	if (_authenCmdMap.find(cmd) != _authenCmdMap.end())
	{
		if (client->containFlags(PASS_FLAG) || client->containFlags(DEBUG_FLAG))
		{
			t_IrcCmd		caller	= _authenCmdMap[cmd];
			t_replyBatch	batch	= caller(this, msg, client);

			applyReplyBatchToClient(batch);
			return;
		}

		// Return Not Registered
		t_replyBatch	batch = errNotRegistered(client);

		applyReplyBatchToClient(batch);
		return;
	}

	// Return Unknow Command
	t_replyBatch	batch = errUnknownCmd(client, cmd);

	applyReplyBatchToClient(batch);
}


