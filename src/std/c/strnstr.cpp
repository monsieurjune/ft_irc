/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strnstr.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 05:09:21 by tponutha          #+#    #+#             */
/*   Updated: 2025/01/24 05:09:22 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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