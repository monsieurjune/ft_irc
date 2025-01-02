// Project Header
#include "std/ft_cppstd.hpp"

// CPP Header
#include <sstream>

namespace ft_std
{

ssize_t	stoi(std::string str)
{
	std::stringstream	ss;
	ssize_t				num = 0;

	// conver string to int
	ss << str;
	ss >> num;

	return num;
}

}
