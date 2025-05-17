/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 17:26:36 by tnualman          #+#    #+#             */
/*   Updated: 2025/05/17 12:28:10 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/FtIrcConstants.hpp"
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"

// C Header
#include <unistd.h>

Client::Client(int const fd): _fd(fd), _timeConnected(std::time(NULL))
{
	_isFlushingToggle = false;
	_authenLevel = 0;

	if (DEBUG_MODE)
	{
		_authenLevel = DEBUG_FLAG;
	}
}

Client::~Client()
{
	close(_fd);
}

int	Client::getFd(void) const
{
	return (_fd);
}

std::time_t	Client::getTimeConnected(void) const
{
	return (_timeConnected);
}

int	Client::getAuthenLevel(void) const
{
	return (_authenLevel);
}

std::string const&	Client::getNickname(void) const
{
	return (_nickname);
}

std::string const&	Client::getUsername(void) const
{
	return (_username);
}

std::string const&	Client::getRealname(void) const
{
	return (_realname);
}

std::string	const&	Client::getHost(void) const
{
	return (_host);
}

std::set<char> const&	Client::getModes(void) const
{
	return (_userMode);
}

bool	Client::getIsFlushing(void) const
{
	return (_isFlushingToggle);
}

bool	Client::hasMode(char c) const
{
	return (_userMode.find(c) != _userMode.end());
}

bool	Client::containFlags(int flags)	const
{
	int	ret	= _authenLevel & flags;

	return (ret == flags);
}

std::string	Client::constructSource()
{
	return ((_username.empty() || _host.empty()) ? _nickname : (_nickname + "!" + _username + "@" + _host));
}

void	Client::setAuthenLevel(int const level)
{
	_authenLevel = level;
}

void	Client::setNickname(std::string const& name)
{
	_nickname = name;
}

void	Client::setUsername(std::string const& name)
{
	_username = name;
}

void	Client::setRealname(std::string const& name)
{
	_realname = name;
}

void	Client::setHost(std::string const& host)
{
	_host = host;
}

void	Client::setIsFlushing()
{
	_isFlushingToggle = true;
}

void	Client::resetIsFlushing()
{
	_isFlushingToggle = false;
}

void	Client::addMode(char c)
{
	_userMode.insert(c);
}

void	Client::removeMode(char c)
{
	_userMode.erase(c);
}

size_t	Client::countReply(void) const
{
	return (_replyQueue.size());
}

void	Client::enqueueReply(std::string const& msg)
{
	_replyQueue.push(msg);
}

std::string	Client::dequeueReply(void)
{
	std::string	msg = _replyQueue.front();

	_replyQueue.pop();
	return (msg);
}
