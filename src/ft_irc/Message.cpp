#include "ft_irc/Message.hpp"

Message::Message(std::string const raw)
{
	// TODO: I suggest that you should put exception or something else here
	parse(raw);
}

Message::~Message(void) {}

// Parser

static std::vector<std::string> split(std::string const & s, std::string const & delimiter) 
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

int Message::parse(std::string const raw)
{
	if (raw.empty()) // Empty message
	{
		_is_valid = false;
		return (1);
	}
	
	std::vector<std::string> raw_splitted = split(raw, " ");
	
	bool has_source = (raw.at(0) == ':');
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
			int temp_pos = has_source ? raw.find(':', 1) : raw.find(':');
			std::string temp = raw.substr(temp_pos + 1, std::string::npos);
			_params.push_back(temp);
			break ;
		}
	}

	_is_valid = true;
	return (0);
}

// Getter

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

bool Message::isValid(void) const
{
	return (_is_valid);
}
