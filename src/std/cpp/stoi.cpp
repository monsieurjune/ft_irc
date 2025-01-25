/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stoi.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 05:09:11 by tponutha          #+#    #+#             */
/*   Updated: 2025/01/24 05:09:12 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "std/ft_cppstd.hpp"

// CPP Header
#include <sstream>

namespace ft_std
{

ssize_t	stoi(std::string str)
{
	std::stringstream	ss;
	ssize_t				num = 0;

	// conver string to int
	ss << str;
	ss >> num;

	return num;
}

size_t		stou(std::string str)
{
	std::stringstream	ss;
	size_t				num = 0;

	// conver string to int
	ss << str;
	ss >> num;

	return num;
}

}
