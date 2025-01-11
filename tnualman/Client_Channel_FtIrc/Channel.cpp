/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 17:40:40 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/11 17:08:43 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string const name): _name(name)
{
	std::time(&_timeCreated);
	std::time(&_timeTopicSet);
	_userCountLimit = -1;
}

Channel::~Channel(void) {}

std::string const & Channel::getName(void) const
{
	return (_name);
}

time_t Channel::getTimeCreated(void) const
{
	return (_timeCreated);
}

int Channel::getUserCount(void) const
{
	return (_userMap.size());
}

bool Channel::hasUser(Client * const client) const
{
	return (_userMap.find(client) != end(_userMap));
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

bool Channel::hasMode(char c) const
{
	return (_modes.find(c) != _modes.end());
}

void Channel::setName(std::string const name)
{
	_name = name;
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

	if (_userMap.find(client) != end(_userMap))
	{
		std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
			<< " already exists on channel " << _name << " !" << std::endl;
		return (1);
	}

	std::set<char> mode_set;

	if (!modestr.empty())
	{
		for (std::string::iterator it = modestr.begin(); it != modestr.end(); it++)
		{
			mode_set.insert(*it);
		}
	}
	_userMap.insert({client, mode_set});
	
	return (0);
}

int	Channel::deleteUserFromChannel(Client * const client)
{
	if (_userMap.find(client) == end(_userMap))
	{
		std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
			<< " does not exist on channel " << _name << " !" << std::endl;
		return (1);
	}
	
	_userMap.erase(client);

	return (0);
}

std::set<char> const & Channel::getMembershipModes(Client * const client) const
{
	try
	{
		return (_userMap.at(client));
	}
	catch (std::exception const & e)
	{
		std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
			<< " not found on channel " << _name << " !" << std::endl;
		return (std::set<char>{'!'});
	}	
}

bool Channel::hasMembershipMode(Client * const client, char const c) const
{
	try
	{
		return (_userMap.at(client).find(c) != end(_userMap.at(client)));
	}
	catch (std::exception const & e)
	{
		std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
			<< " not found on channel " << _name << " !" << std::endl;
		return (false);
	}	
}

char Channel::addMembershipMode(Client * const client, char const c)
{
	try
	{
		std::set<char> & mode_set = _userMap.at(client); // throws if user is not found
		mode_set.insert(c);
		return (c);
	}
	catch (std::exception const & e)
	{
		std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
			<< " not found on channel " << _name << " !" << std::endl;
		return (NULL);
	}	
}

char Channel::removeMembershipMode(Client * const client, char const c)
{
	try
	{
		std::set<char> & mode_set = _userMap.at(client); // throws if user is not found
		mode_set.erase(c);
		return (c);
	}
	catch (std::exception const & e)
	{
		std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
			<< " not found on channel " << _name << " !" << std::endl;
		return (NULL);
	}	
}

int	Channel::addMembershipMode(Client * const client, std::string s)
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

int	Channel::removeMembershipMode(Client * const client, std::string s)
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
