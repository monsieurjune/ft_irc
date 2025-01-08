#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

// CPP Header
#include <string>
#include <vector>
#include <map>

class Message
{
	private:
		std::string					_prefix;
		std::string					_command;
		std::vector<std::string>	_params;
		bool						_is_valid;

	public:
		Message(std::string const raw);
		
		~Message(void);
		
		// Returns status value, implemented separately from the constructor for the sake of modularity,
		// in case we add something to the constructor later.
		int	parse(std::string const raw);  

		// Getters
		std::string							getSource(void) const;
		std::string							getCommand(void) const;
		std::vector<std::string>			getParams(void) const;
		bool								isValid(void) const;

		// Setters
};

#endif