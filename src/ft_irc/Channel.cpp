#include "ft_irc/Channel.hpp"

Channel::Channel(std::string const name): _name(name)
{
	std::time(&_timeCreated);
	std::time(&_timeTopicSet);
	_userCountLimit = -1;
}

Channel::~Channel(void) {}

// Getter

std::string const & Channel::getName(void) const
{
	return (_name);
}

int Channel::getUserCount(void) const
{
	// TODO: Change to something else
	return (0);
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

// Setter

void Channel::setName(std::string const name)
{
	_name = name;
}

void Channel::setTopic(std::string const topic, Client const * const client)
{
	_topic = topic;
	
	// TODO: We don't parse NULL in client param, right?
	// In that case, i think if-else below is useless

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
