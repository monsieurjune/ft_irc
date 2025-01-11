#include "ft_irc/Client.hpp"
#include <unistd.h>

Client::Client(int const fd): _fd(fd)
{
	std::time(&_timeConnected);
}

Client::~Client(void)
{
	close(_fd);
}

// Getter

int Client::getFd(void) const
{
	return (_fd);
}

int	Client::getAuthorizeLevel(void) const
{
	return (_authorizeLevel);
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

// Setter

void	Client::setAuthorizeLevel(int const level)
{
	_authorizeLevel = level;
}

void	Client::setNickname(std::string const name)
{
	_nickname = name;
}

void	Client::setUsername(std::string const name)
{
	_username = name;
}

void	Client::setRealname(std::string const realname)
{
	_realname = realname;
}

void	Client::setHost(std::string const str)
{
	_host = str;
}
