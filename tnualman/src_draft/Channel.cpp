/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 17:40:40 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/28 15:42:09 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string const name, Client * const creator)
	: _name(name), _timeCreated(time(NULL)), _timeTopicSet(time(NULL)),
		_userCountLimit(0), _owner(creator) {}

Channel::~Channel(void) {}

std::string const & Channel::getName(void) const
{
	return (_name);
}

std::string const & Channel::getPassword(void) const
{
	return (_password);
}

Client * const Channel::getOwner(void) const
{
	return (_owner);
}

time_t Channel::getTimeCreated(void) const
{
	return (_timeCreated);
}

Channel::t_userMap const & Channel::getUserMap(void) const
{
	return (_userMap);
}

int Channel::getUserCount(void) const
{
	return (_userMap.size());
}

int Channel::getUserCountLimit(void) const
{
	return (_userCountLimit);
}

std::string const & Channel::getTopic(void) const
{
	return (_topic);
}

time_t Channel::getTimeTopicSet(void) const
{
	return (_timeTopicSet);
}

std::string const & Channel::getTopicSetter(void) const
{
	return (_topicSetter);
}

std::set<char> const & Channel::getModes(void) const
{
	return (_modes);
}

bool Channel::hasThisClient(Client * const client) const
{
	return (_userMap.find(client) != _userMap.end());
}

bool Channel::hasThisMode(char c) const
{
	return (_modes.find(c) != _modes.end());
}

void Channel::setName(std::string const name)
{
	_name = name;
}

void Channel::setPassword(std::string const password)
{
	_password = password;
}

void Channel::setOwner(Client * const owner)
{
	_owner = owner;
}

void Channel::setUserCountLimit(int const limit)
{
	_userCountLimit = limit;
}

void Channel::setTopic(std::string const topic, Client const * const client)
{
	_topic = topic;
	
	if (client)
	{
		_topicSetter = client->getNickname();
	}
	else
	{
		_topicSetter = "<unspecified>";
	}

	std::time(&_timeTopicSet);
}

void Channel::addMode(char const c)
{
	_modes.insert(c);
}

void Channel::removeMode(char const c)
{
	_modes.erase(c);
}

void Channel::addMode(std::string s)
{
	if (!s.empty())
	{
		for (std::string::iterator it = s.begin(); it != s.end(); it++)
		{
			_modes.insert(*it);
		}
	}
}

void Channel::removeMode(std::string s)
{
	if (!s.empty())
	{
		for (std::string::iterator it = s.begin(); it != s.end(); it++)
		{
			_modes.erase(*it);
		}
	}
}

int Channel::addUserToChannel(Client * const client, std::string modestr)
{

	if (_userMap.find(client) != _userMap.end())
	{
		std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
			<< " already exists on channel " << _name << " !" << std::endl;
		return (1);
	}

	if (hasThisMode('l') && getUserCount() >= getUserCountLimit())
	{
		std::cerr << "Channel's user count limit reached!" << std::endl;
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
		std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
			<< " does not exist on channel " << _name << " !" << std::endl;
		return (1);
	}
	
	_userMap.erase(client);

	return (0);
}

std::set<char> const & Channel::getThisClientMembershipModes(Client * const client) const
{
	try
	{
		return (_userMap.at(client));
	}
	catch (std::exception const & e)
	{
		std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
			<< " not found on channel " << _name << " !" << std::endl;
		std::set<char> ret;
		ret.insert('!');
		return (ret);
	}	
}

bool Channel::hasThisClientMembershipMode(Client * const client, char const c) const
{
	try
	{
		return (_userMap.at(client).find(c) != _userMap.at(client).end());
	}
	catch (std::exception const & e)
	{
		std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
			<< " not found on channel " << _name << " !" << std::endl;
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
		std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
			<< " not found on channel " << _name << " !" << std::endl;
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
		std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
			<< " not found on channel " << _name << " !" << std::endl;
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
		std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
			<< " not found on channel " << _name << " !" << std::endl;
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
	}
	catch (std::exception const & e)
	{
		std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
			<< " not found on channel " << _name << " !" << std::endl;
		return (-1);
	}
}
