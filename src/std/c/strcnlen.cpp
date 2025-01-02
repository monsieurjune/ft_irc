#include "std/ft_cstd.hpp"

namespace ft_std
{

size_t	strcnlen(const char *str, int c, size_t n)
{
	size_t	i = 0;

	while (i < n && str[i] != '\0')
	{
		if (str[i] == c)
		{
			return i;
		}
		i++;
	}
	return i;
}

}