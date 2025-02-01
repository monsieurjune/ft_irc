/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 17:26:36 by tnualman          #+#    #+#             */
/*   Updated: 2025/02/01 06:02:41 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/Client.hpp"
#include "ft_irc/Channel.hpp"

Channel::Channel(std::string const name, Client * const creator) : _name(name), \
																	_timeCreated(std::time(NULL)), \
																	_userCountLimit(0)
{
	// Add creator as first operator of channel
	addUserToChannel(creator, std::string(1, MODE_OPERATOR));

	// Add certain mode that setted by default (i.e. topic mode)
	addMode(MODE_PROTECTTOPIC);
}

Channel::~Channel()
{
	// Super edge case, which shouldn't happend in the first place
	// i.e. Channel is destroyed while still has users
	if (_userMap.size() > 0)
	{
		// TODO: might handle later
	}
}

std::string const&	Channel::getName(void) const
{
	return (_name);
}

std::string const&	Channel::getPassword(void) const
{
	return (_password);
}

std::time_t	Channel::getTimeCreated(void) const
{
	return (_timeCreated);
}

size_t	Channel::getUserCount(void) const
{
	return (_userMap.size());
}

size_t Channel::getUserCountLimit(void) const
{
	return (_userCountLimit);
}

std::string const&	Channel::getTopic(void) const
{
	return (_topic);
}

std::time_t	Channel::getTimeTopicSet(void) const
{
	return (_timeTopicSet);
}

std::string const&	Channel::getTopicSetter(void) const
{
	return (_topicSetter);
}

std::set<char> const&	Channel::getModes(void) const
{
	return (_modes);
}

bool	Channel::hasThisClient(Client * const client) const
{
	return (_userMap.find(client) != _userMap.end());
}

bool	Channel::hasThisMode(char c) const
{
	return (_modes.find(c) != _modes.end());
}

void	Channel::setPassword(std::string const password)
{
	_password = password;
}

void	Channel::setUserCountLimit(size_t const limit)
{
	_userCountLimit = limit;
}

void	Channel::setTopic(std::string const topic, Client const * const setter)
{
	_topic = topic;
	_topicSetter = setter->getNickname();
	std::time(&_timeTopicSet);
}

void	Channel::addMode(char const c)
{
	_modes.insert(c);
}

void	Channel::addMode(std::string s)
{
	if (s.empty())
	{
		return;
	}
	
	for (std::string::iterator it = s.begin(); it != s.end(); it++)
	{
		_modes.insert(*it);
	}
}

void	Channel::removeMode(char const c)
{
	_modes.erase(c);
}

void	Channel::removeMode(std::string s)
{
	if (s.empty())
	{
		return;
	}
	
	for (std::string::iterator it = s.begin(); it != s.end(); it++)
	{
		_modes.erase(*it);
	}
}

int	Channel::addUserToChannel(Client * const client, std::string modestr)
{

	if (_userMap.find(client) != _userMap.end())
	{
		// std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
		// 	<< " already exists on channel " << _name << " !" << std::endl;
		return (1);
	}

	if (hasThisMode(MODE_USERLIMIT) && getUserCount() >= getUserCountLimit())
	{
		// std::cerr << "Channel's user count limit reached!" << std::endl;
		return (2);
	}
	
	std::set<char> mode_set;

	if (!modestr.empty())
	{
		for (std::string::iterator it = modestr.begin(); it != modestr.end(); it++)
		{
			mode_set.insert(*it);
		}
	}
	_userMap[client] = mode_set;
	
	return (0);
}

int	Channel::deleteUserFromChannel(Client * const client)
{
	if (_userMap.find(client) == _userMap.end())
	{
		// std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
		// 	<< " does not exist on channel " << _name << " !" << std::endl;
		return (1);
	}
	
	_userMap.erase(client);

	return (0);
}


std::set<char>	Channel::getThisClientMembershipModes(Client * const client) const
{
	try
	{
		return (_userMap.at(client));
	}
	catch (std::exception const & e)
	{
		std::set<char> ret;

		// std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
		// 	<< " not found on channel " << _name << " !" << std::endl;
		ret.insert(MODE_RESERVE_ERROR);
		return (ret);
	}	
}

bool	Channel::hasThisClientMembershipMode(Client * const client, char const c) const
{
	try
	{
		return (_userMap.at(client).find(c) != _userMap.at(client).end());
	}
	catch (std::exception const & e)
	{
		// std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
		// 	<< " not found on channel " << _name << " !" << std::endl;
		return (false);
	}	
}

int	Channel::addThisClientMembershipMode(Client * const client, char const c)
{
	try
	{
		std::set<char> & mode_set = _userMap.at(client); // throws if user is not found
		mode_set.insert(c);
		return (0);
	}
	catch (std::exception const & e)
	{
		// std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
		// 	<< " not found on channel " << _name << " !" << std::endl;
		return (-1);
	}	
}

int	Channel::addThisClientMembershipMode(Client * const client, std::string s)
{
	try
	{
		std::set<char> & mode_set = _userMap.at(client); // throws if user is not found
		if (!s.empty())
		{
			for (std::string::iterator it = s.begin(); it != s.end(); it++)
			{
				mode_set.insert(*it);
			}
		}
		return (0);
	}
	catch (std::exception const & e)
	{
		// std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
		// 	<< " not found on channel " << _name << " !" << std::endl;
		return (-1);
	}
}

int	Channel::removeThisClientMembershipMode(Client * const client, char const c)
{
	try
	{
		std::set<char> & mode_set = _userMap.at(client); // throws if user is not found
		mode_set.erase(c);
		return (0);
	}
	catch (std::exception const & e)
	{
		// std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
		// 	<< " not found on channel " << _name << " !" << std::endl;
		return (-1);
	}	
}

int	Channel::removeThisClientMembershipMode(Client * const client, std::string s)
{
	try
	{
		std::set<char> & mode_set = _userMap.at(client); // throws if user is not found
		if (!s.empty())
		{
			for (std::string::iterator it = s.begin(); it != s.end(); it++)
			{
				mode_set.erase(*it);
			}
			return (0);
		}
		return (-1);
	}
	catch (std::exception const & e)
	{
		// std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
		// 	<< " not found on channel " << _name << " !" << std::endl;
		return (-1);
	}
}
