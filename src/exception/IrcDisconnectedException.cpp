#include "exception/IrcDisconnectedException.hpp"

IrcDisconnectedException::IrcDisconnectedException(const char *msg) : exception()
{
	_msg = std::string(msg);
}

IrcDisconnectedException::~IrcDisconnectedException() throw() {}

const char *IrcDisconnectedException::what() const throw()
{
	return _msg.c_str();
}