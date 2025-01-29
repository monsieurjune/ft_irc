/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strcnlen.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 05:09:24 by tponutha          #+#    #+#             */
/*   Updated: 2025/01/30 06:02:22 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "std/ft_cstd.hpp"

namespace ft_std
{

ssize_t	strcnlen(const char *str, int c, ssize_t n)
{
	ssize_t	i = 0;

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