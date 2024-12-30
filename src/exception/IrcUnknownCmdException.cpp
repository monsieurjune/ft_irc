#include "exception/IrcUnknownCmdException.hpp"

IrcUnknownCmdException::IrcUnknownCmdException(const char *msg)
{
	_msg = std::string(msg);
}

const char *IrcUnknownCmdException::what() const throw()
{
	return _msg.c_str();
}