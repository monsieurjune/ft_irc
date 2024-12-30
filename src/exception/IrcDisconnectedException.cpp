#include "exception/IrcDisconnectedException.hpp"

IrcDisconnectedException::IrcDisconnectedException(const char *msg)
{
	_msg = std::string(msg);
}

const char *IrcDisconnectedException::what() const throw()
{
	return _msg.c_str();
}