#include "exception/CstdException.hpp"
#include "std/ft_cppstd.hpp"

CstdException::CstdException(int code)
{
	_msg = ft_std::itoa(code);
}

const char *CstdException::what() const throw()
{
	return _msg.c_str();
}