/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:13:51 by tponutha          #+#    #+#             */
/*   Updated: 2025/02/01 14:04:40 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/ft_utils.hpp"

#include <string>
#include <iostream>

namespace ft_utils
{

void	logger(std::string name, std::string msg)
{
	std::cout << "[" << name << "] " << msg << std::endl;
}

}