/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log10i.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 06:15:02 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/19 06:44:16 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "std/ft_cstd.hpp"
#include <cmath>

namespace ft_std
{

int     log10i(ssize_t num)
{
    return static_cast<int>(std::floor(std::log10(num)));
}

}