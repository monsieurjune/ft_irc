#ifndef __IRC_UNKNOWN_CMD_EXCEPTION_HPP__
#define __IRC_UNKNOWN_CMD_EXCEPTION_HPP__
#include <exception>
#include <string>

class IrcUnknownCmdException : public std::exception
{
	private:
		std::string	_msg;

	public:
		IrcUnknownCmdException(const char *msg);
		virtual ~IrcUnknownCmdException() throw();
		virtual const char* what() const throw();
};

#endif