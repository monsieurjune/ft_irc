/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 17:26:36 by tnualman          #+#    #+#             */
/*   Updated: 2024/12/24 21:14:36 by tnualman         ###   ########.fr       */
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

Message::Message(std::string const raw): _raw(raw)
{
	parse();
}

Message::Message(char * const raw): _raw(raw)
{
	parse();
}

Message::~Message(void) {}

int Message::parse(void)
{
	if (_raw.empty()) // Empty message
	{
		_is_valid = false;
		return (1);
	}
	// if (_raw.find("\r\n") == std::string::npos) // Message has no \r\n
	// {
	// 	_is_valid = false;
	// 	return (1);
	// }
	
	// _raw = _raw.substr(0, _raw.find("\r\n")); // Discard everything after \r\n
	
	std::vector<std::string> raw_splitted = split(_raw, " ");
	
	bool has_source = (_raw.at(0) == ':');
	int param_idx;	
	
	if (has_source) // Message has a source/prefix
	{
		if (raw_splitted.size() == 1)
		{
			_is_valid = false;
			return (1);
		}
		_source = raw_splitted.at(0).substr(1, std::string::npos); // Trim lreading ':'
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
			int temp_pos = has_source ? _raw.find(':', 1) : _raw.find(':');
			std::string temp = _raw.substr(temp_pos + 1, _raw.find("\r\n"));
			_params.push_back(temp);
			break ;
		}
	}
	_is_valid = true;
	return (0);
}

std::string Message::getRawMessage(void) const
{
	return (_raw);
}

std::string Message::getSource(void) const
{
	return (_source);
}

std::string Message::getCommand(void) const
{
	return (_command);
}

std::vector<std::string> Message::getParams(void) const
{
	return (_params);
}

bool Message::getIsValid(void) const
{
	return (_is_valid);
}
