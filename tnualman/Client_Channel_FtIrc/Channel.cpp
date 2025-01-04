/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 17:40:40 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/04 20:15:37 by tnualman         ###   ########.fr       */
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

Channel::t_userFlags Channel::getUserFlags(Client * const client) const
{
	try
	{
		return (_userMap.at(client));
	}
	catch (std::exception const & e)
	{
		std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
			<< " not found on channel " << _name << " !" << std::endl;
		return (t_userFlags(0b0001));
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

Channel::t_userFlags Channel::setUserFlags(Client * const client, Channel::t_userFlags const & flags)
{
	try
	{
		// TODO: VERIFY THIS LOGIC, READ THE DOCS FOR std::bitmap !!
		return (_userMap.at(client) = flags);
	}
	catch (std::exception const & e)
	{
		std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
			<< " not found on channel " << _name << " !" << std::endl;
		return (t_userFlags(0b0001));
	}	
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

int Channel::addUserToChannel(Client * const client, int const status)
{

	if (_userMap.find(client) == end(_userMap))
	{
		std::cerr << "Client named " << client->getNickname() << ", socket " << client->getFd()
			<< " already exists on channel " << _name << " !" << std::endl;
		return (-1);
	}

	_userMap.insert({client, status});

	return (0);
}