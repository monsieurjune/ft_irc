/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 00:23:30 by tnualman          #+#    #+#             */
/*   Updated: 2024/12/29 02:59:57 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void)
{
	// Does this happened at construction time?
	std::time(&_timeConnected);
}

Client::Client(int const fd): _fd(fd)
{
	// Does this happened at construction time?
	std::time(&_timeConnected);
}

Client::Client(int const fd, std::string const nickname,
				std::string const username, std::string const host)
				: _fd(fd), _nickname(nickname), _username(username), _host(host) {}

Client::~Client(void) {}

int Client::getFd(void) const
{
	return (_fd);
}

int Client::getAuthorizeLevel(void) const
{
	return (_authorizeLevel);
}

std::string Client::getNickname(void) const
{
	return (_nickname);
}

std::string Client::getUsername(void) const
{
	return (_username);
}

std::string	Client::getHost(void) const
{
	return (_host);
}

void Client::setAuthorizeLevel(int const level)
{
	_authorizeLevel = level;
}
void Client::setNickname(std::string const name)
{
	_nickname = name;
}

void Client::setUsername(std::string const name)
{
	_username = name;
}
void Client::setHost(std::string const str)
{
	_host = str;
}
