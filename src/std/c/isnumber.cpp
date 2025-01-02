#include "std/ft_cstd.hpp"

namespace ft_std
{

bool	isnumber(const char *num)
{
	size_t	i = 0;

	while (num[i] != '\0')
	{
		if (num[i] < '0' || num[i] > '9')
		{
			return false;
		}
		i++;
	}
	return true;
}

}