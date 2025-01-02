// Project Header
#include "std/ft_cppstd.hpp"

// CPP Header
#include <sstream>

namespace ft_std
{

std::string	itoa(ssize_t num)
{
	std::ostringstream oss;

	oss << num;
	return oss.str();
}

}