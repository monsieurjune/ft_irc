#ifndef __FT_CSTD_HPP__
#define __FT_CSTD_HPP__
#include <sys/types.h>
#include <cstddef>

namespace ft_std
{

void	*memset(void *ptr, int c, size_t byte);
bool	isnumber(const char *num);

}

#endif