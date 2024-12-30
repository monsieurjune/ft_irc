#include "utils/ft_utils.hpp"

namespace ft_utils
{

void	*memset(void *ptr, int c, size_t byte)
{
	size_t	i = 0;

	if (!ptr)
		return (NULL);
	while (i < byte)
	{
		((unsigned char *)ptr)[i] = ((unsigned char *)&c)[0];
		i++;
	}
	return (ptr);
}

}