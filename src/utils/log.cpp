#include "utils/ft_utils.hpp"

#include <string>
#include <iostream>

namespace ft_utils
{

void	logger(std::string name, std::string msg)
{
	std::cout << "[" << name << "] " << msg << std::endl;
}

}