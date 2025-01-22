/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 17:26:36 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/22 21:16:10 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

std::vector<std::string> split(std::string const & s, std::string const & delimiter) 
{
    std::string tmp = s;
	std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = tmp.find(delimiter)) != std::string::npos)
	{
        token = tmp.substr(0, pos);
        tokens.push_back(token);
        tmp.erase(0, pos + delimiter.length());
    }
    tokens.push_back(tmp);

    return (tokens);
}

Message::Message(void) {}

Message::Message(Message const & origin)
{
	*this = origin;
}

Message & Message::operator=(Message const & rhs)
{
	_source = rhs._source;
	_command = rhs._command;
	_params = rhs._params;
	_isValid = rhs._isValid;

	return (*this);
} 

Message::Message(std::string const raw)
{
	parse(raw);
}

Message::~Message(void) {}

int Message::parse(std::string const raw)
{
	if (raw.empty()) // Empty message
	{
		_isValid = false;
		return (1);
	}
	// if (_raw.find("\r\n") == std::string::npos) // Message has no \r\n
	// {
	// 	_is_valid = false;
	// 	return (1);
	// }
	
	// _raw = _raw.substr(0, _raw.find("\r\n")); // Discard everything after \r\n
	
	std::vector<std::string> raw_splitted = split(raw, " ");
	
	bool has_source = (raw.at(0) == ':');
	int param_idx;	
	
	if (has_source) // Message has a source/prefix
	{
		if (raw_splitted.size() == 1)
		{
			_isValid = false;
			return (1);
		}
		_source = raw_splitted.at(0).substr(1, std::string::npos); // Trim lreading ':'
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
	while (param_idx < (int)raw_splitted.size())
	{
		if (raw_splitted.at(param_idx).at(0) != ':') // Not yet the trailing param...
		{
			_params.push_back(raw_splitted.at(param_idx++));
		}
		else // IS the trailing param
		{
			int temp_pos = has_source ? raw.find(':', 1) : raw.find(':');
			std::string const temp = raw.substr(temp_pos + 1, std::string::npos);
			_params.push_back(temp);
			break ;
		}
	}
	_isValid = true;
	return (0);
}

std::string const & Message::getSource(void) const
{
	return (_source);
}

std::string const & Message::getCommand(void) const
{
	return (_command);
}

std::vector<std::string> const & Message::getParams(void) const
{
	return (_params);
}

bool Message::isValid(void) const
{
	return (_isValid);
}

std::string const & Message::getMessage(void)
{
	std::string msg;

	if (!_source.empty())
	{
		msg += ":" + _source + " ";
	}
	
	msg += _command;
	
	for (int i = 0; i < _params.size(); i++)
	{
		msg += " ";
		if (i == _params.size() - 1 && (_params.at(i).empty() || _params.at(i).find(' ') != std::string::npos))
		{
			msg += ":";
		}
		msg += _params.at(i);
	}

	_isValid = true;
	return (msg);
}

void Message::setSource(std::string src)
{
	_source = src;
}

void Message::setCommand(std::string cmd)
{
	_command = cmd;
}

void Message::setCommand(int cmd)
{
	std::stringstream ss;

	ss << cmd;
	_command = ss.str();
}

void Message::resetParams(void)
{
	_params.clear();
	_params.resize(0);
}

void Message::pushParam(std::string param)
{
	_params.push_back(param);
}
