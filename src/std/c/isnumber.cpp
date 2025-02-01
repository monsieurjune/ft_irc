/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isnumber.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 05:09:30 by tponutha          #+#    #+#             */
/*   Updated: 2025/02/01 14:04:44 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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