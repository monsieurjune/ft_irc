#ifndef __FT_CPPSTD_HPP__
#define __FT_CPPSTD_HPP__
#include <sys/types.h>
#include <string>

namespace ft_std
{

ssize_t		stoi(std::string str);
std::string	itoa(ssize_t num);

}

#endif