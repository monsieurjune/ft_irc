#include "exception/IrcInvalidPacketException.hpp"

IrcInvalidPacketException::IrcInvalidPacketException(const char *msg)
{
	_msg = std::string(msg);
}

const char *IrcInvalidPacketException::what() const throw()
{
	return _msg.c_str();
}