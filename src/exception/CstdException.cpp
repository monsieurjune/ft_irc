#include "exception/CstdException.hpp"

CstdException::CstdException(int code)
{
	_msg = code;
}