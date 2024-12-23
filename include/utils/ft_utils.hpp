#ifndef __FT_UTILS_HPP__
#define __FT_UTILS_HPP__
#include <string>

namespace ft_utils
{

long	stoi(std::string str);
bool	is_number(const char *num);
void	*memset(void *ptr, int c, size_t byte);
void	logger(std::string name, std::string msg);

}

#endif