/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 05:09:16 by tponutha          #+#    #+#             */
/*   Updated: 2025/02/01 14:04:50 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "std/ft_cppstd.hpp"

// CPP Header
#include <sstream>

namespace ft_std
{

std::string	itoa(ssize_t num)
{
	std::ostringstream oss;

	oss << num;
	return oss.str();
}

}