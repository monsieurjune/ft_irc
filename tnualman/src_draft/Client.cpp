/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 00:23:30 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/25 16:38:27 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int const fd): _fd(fd)
{
	// Does this happened at construction time?
	std::time(&_timeConnected);
}

Client::~Client(void) {}

int Client::getFd(void) const
{
	return (_fd);
}

time_t const Client::getTimeConnected(void) const
{
	return (_timeConnected);
}

int Client::getAuthenLevel(void) const
{
	return (_authenLevel);
}

std::string const & Client::getNickname(void) const
{
	return (_nickname);
}

std::string const & Client::getUsername(void) const
{
	return (_username);
}

std::string const & Client::getRealname(void) const
{
	return (_realname);
}

std::string	const & Client::getHost(void) const
{
	return (_host);
}

std::set<Channel*> const & Client::getChannels(void) const
{
	return (_channels);
}

std::set<char> const & Client::getModes(void) const
{
	return (_modes);
}

bool Client::hasMode(char c) const
{
	return (_modes.find(c) != _modes.end());
}

bool Client::isInChannel(Channel * const channel)
{
	return(_channels.find(channel) != _channels.end());
}

void Client::setAuthenLevel(int const level)
{
	_authenLevel = level;
}
void Client::setNickname(std::string const name)
{
	_nickname = name;
}

void Client::setUsername(std::string const name)
{
	_username = name;
}

void Client::setRealname(std::string const name)
{
	_realname = name;
}

void Client::setHost(std::string const str)
{
	_host = str;
}

void Client::addMode(char c)
{
	_modes.insert(c);
}

void Client::removeMode(char c)
{
	_modes.erase(c);
}

void Client::editChannelSet(Channel * const channel, bool add)
{
	if (add)
	{
		_channels.insert(channel);
	}
	else
	{
		_channels.erase(channel);
	}
}

size_t Client::countReply(void)
{
	return (_replyQueue.size());
}

void Client::enqueueReply(std::string const msg)
{
	_replyQueue.push(msg);
}

std::string const &	Client::dequeueReply(void)
{
	std::string const msg = _replyQueue.front();
	_replyQueue.pop();
	return (msg);
}
