#ifndef __CSTD_EXCEPTION_HPP__
#define __CSTD_EXCEPTION_HPP__
#include <exception>
#include <string>

class CstdException : public std::exception
{
	private:
		std::string	msg;

	public:
		CstdException();
		virtual const char* what() const throw();
};

#endif