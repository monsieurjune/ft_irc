#include "std/ft_cstd.hpp"

static bool	sub_check(const char *str, const char *find, size_t i, size_t n)
{
	size_t	j;

	j = 0;
	while (find[j] && i + j < n)
	{
		if (str[i + j] != find[j])
			return (0);
		j++;
	}
	return (find[j] == 0);
}

namespace ft_std
{

char	*strnstr(const char *str, const char *find, size_t n)
{
	size_t	i;

	if (!find || !find[0])
		return ((char *)str);
	if (!str)
		return (NULL);
	i = 0;
	while (str[i] && i < n)
	{
		if (str[i] == find[0])
			if (sub_check(str, find, i, n))
				return ((char *)&str[i]);
		i++;
	}
	return (NULL);
}

}