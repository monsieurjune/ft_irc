#include "exception/IrcUnknownCmdException.hpp"

IrcUnknownCmdException::IrcUnknownCmdException(const char *msg) : exception()
{
	_msg = std::string(msg);
}

IrcUnknownCmdException::~IrcUnknownCmdException() throw() {}

const char *IrcUnknownCmdException::what() const throw()
{
	return _msg.c_str();
}