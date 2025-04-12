/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 17:26:36 by tnualman          #+#    #+#             */
/*   Updated: 2025/04/13 06:00:35 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "ft_irc/Message.hpp"
#include "std/ft_cppstd.hpp"

// orthodox

Message::Message(void): _isValid(true), _hasTrailing(false) {}

Message::Message(Message const & origin)
{
	*this = origin;
}

Message& Message::operator=(Message const & rhs)
{
	_source = rhs._source;
	_command = rhs._command;
	_params = rhs._params;
	_isValid = rhs._isValid;

	return (*this);
}

Message::Message(std::string const raw)
{
	_hasTrailing = false;

	if (parse(raw) == 0)
	{
		_isValid = true;
	}
	else
	{
		_isValid = false;
	}
}

Message::~Message(void) {}

// parse

int Message::parse(std::string const raw)
{
	// Empty message
	if (raw.empty())
	{
		return (1);
	}

	std::vector<std::string> raw_splitted = ft_std::split(raw, " ");

	bool	has_source = (raw.at(0) == ':');
	size_t	param_idx;	

	// Message has a source/prefix
	if (has_source)
	{
		if (raw_splitted.size() == 1)
		{
			return (1);
		}

		// Trim lreading ':'
		_source = raw_splitted.at(0).substr(1, std::string::npos);

		// Parse _source here if it needs to be done.
		_command = raw_splitted.at(1);
		param_idx = 2;
	}
	else
	{
		_command = raw_splitted.at(0);
		param_idx = 1;
	}

	// Looping over the rest to get params
	while (param_idx < raw_splitted.size())
	{
		if (raw_splitted.at(param_idx).at(0) != ':')
		{
			// Not yet the trailing param...
			_params.push_back(raw_splitted.at(param_idx++));
		}
		else
		{
			// IS the trailing param
			size_t				temp_pos = has_source ? raw.find(':', 1) : raw.find(':');
			std::string const	temp = raw.substr(temp_pos + 1, std::string::npos);

			_params.push_back(temp);
			_hasTrailing = true;
			break ;
		}
	}
	return (0);
}

// getter

std::string const&	Message::getSource(void) const
{
	return (_source);
}

std::string const&	Message::getCommand(void) const
{
	return (_command);
}

std::vector<std::string> const&	Message::getParams(void) const
{
	return (_params);
}

bool	Message::isValid(void) const
{
	return (_isValid);
}

bool	Message::hasTrailing(void) const
{
	return (_hasTrailing);
}

// assembler

std::string	Message::assembleRawMessage(void)
{
	std::string msg;

	// Reserve the memory
	msg.reserve(IRC_MSG_MAXSIZE);

	if (!_source.empty())
	{
		msg += ":" + _source + " ";
	}
	
	msg += _command;
	
	for (size_t i = 0; i < _params.size(); i++)
	{
		msg += " ";
		if (i == _params.size() - 1 && (_params.at(i).empty() || _params.at(i).find(' ') != std::string::npos))
		{
			_hasTrailing = true;
			msg += ":";
		}
		msg += _params.at(i);
	}

	// Cut out exceed characters to fix IRC standard (excluding \r\n)
	if (msg.length() > IRC_MSG_MAXSIZE - 2)
	{
		msg.resize(IRC_MSG_MAXSIZE - 2);
	}

	// Add IRC Terminate Bytes at the end
	msg += IRC_TERMINATE_BYTES;

	_isValid = true;
	return (msg);
}

// setter

void Message::setSource(std::string src)
{
	_source = src;
}

void Message::setCommand(std::string cmd)
{
	_command = cmd;
}

void Message::setCommand(e_numerics cmd)
{
	int	code = static_cast<int>(cmd);

	_command = ft_std::itoa(code);
}

void Message::resetParams(void)
{
	_params.clear();
}

void Message::pushParam(std::string param)
{
	_params.push_back(param);
}
