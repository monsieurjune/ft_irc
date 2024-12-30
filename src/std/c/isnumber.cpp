#include "utils/ft_utils.hpp"

namespace ft_utils
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