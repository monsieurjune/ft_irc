#include "utils/ft_utils.hpp"
#include <sstream>

namespace ft_utils
{

std::string	itoa(ssize_t num)
{
	std::ostringstream oss;

	oss << num;
	return oss.str();
}

}