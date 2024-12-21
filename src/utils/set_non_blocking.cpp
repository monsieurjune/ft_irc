#include "utils/ft_utils.hpp"

#include <fcntl.h>

namespace ft_utils
{

int		set_non_blocking(int fd)
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
	{
		return -1;
	}
	return fd;
}

}