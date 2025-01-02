#ifndef __CSTD_EXCEPTION_HPP__
#define __CSTD_EXCEPTION_HPP__
#include <exception>
#include <string>

class CstdException : public std::exception
{
	private:
		std::string	_msg;

	public:
		CstdException(int code);
		virtual ~CstdException() throw();
		virtual const char* what() const throw();
};

#endif