#ifndef __IRC_DISCONNECTED_EXCEPTION_HPP__
#define __IRC_DISCONNECTED_EXCEPTION_HPP__
#include <exception>
#include <string>

class IrcDisconnectedException : public std::exception
{
	private:
		std::string	msg;

	public:
		IrcDisconnectedException();
		virtual const char* what() const throw();
};

#endif