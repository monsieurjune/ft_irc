/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 17:40:40 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/07 20:05:31 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(void)
{
	std::time(&_timeCreated);
	std::time(&_timeTopicSet);
	_userCountLimit = -1;
}

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

std::string const & Channel::getUserMembership(Client * const client) const
{
	try
	{
		return (_userMap.at(client));
	}
	catch (std::exception const & e)
	{
		std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
			<< " not found on channel " << _name << " !" << std::endl;
		return ("!");
	}	
}

int Channel::getUserCount(void) const
{
	return (_userMap.size());
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

void Channel::setName(std::string const name)
{
	_name = name;
}

char Channel::addMembershipMode(Client * const client, char mode)
{
	try
	{
		std::string & modestr = _userMap.at(client); // throws if user is not found
		if (modestr.find(mode) != std::string::npos)
		{
			modestr.append(1, mode);
		}
		return (mode);
	}
	catch (std::exception const & e)
	{
		std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
			<< " not found on channel " << _name << " !" << std::endl;
		return (NULL);
	}	
}

char Channel::removeMembershipMode(Client * const client, char mode)
{
	// try
	// {
	// 	std::string & modestr = _userMap.at(client); // throws if user is not found
	// 	if (modestr.find(mode) != std::string::npos)
	// 	{
	// 		modestr.append(1, mode);
	// 	}
	// 	return (mode);
	// }
	// catch (std::exception const & e)
	// {
	// 	std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
	// 		<< " not found on channel " << _name << " !" << std::endl;
	// 	return (NULL);
	// }
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

int Channel::addUserToChannel(Client * const client, std::string const & modestr)
{

	if (_userMap.find(client) == end(_userMap))
	{
		std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
			<< " already exists on channel " << _name << " !" << std::endl;
		return (1);
	}

	_userMap.insert({client, modestr});

	return (0);
}