#include "exception/IrcInvalidPacketException.hpp"

IrcInvalidPacketException::IrcInvalidPacketException(const char *msg) : exception()
{
	_msg = std::string(msg);
}

IrcInvalidPacketException::~IrcInvalidPacketException() throw() {}

const char *IrcInvalidPacketException::what() const throw()
{
	return _msg.c_str();
}