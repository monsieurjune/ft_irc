// Project Header
#include "utils/ft_utils.hpp"

// CPP Header
#include <sstream>

namespace ft_utils
{

long	stoi(std::string str)
{
	std::stringstream	ss;
	long				num = 0;

	// conver string to int
	ss << str;
	ss >> num;

	return num;
}

}
