/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:01:25 by tponutha          #+#    #+#             */
/*   Updated: 2025/02/01 14:03:36 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FT_UTILS_HPP__
#define __FT_UTILS_HPP__

// CPP Header
#include <string>

namespace ft_utils
{

void	logger(std::string name, std::string msg);
int		signal_init(void (*killer)(int), void (*ignorer)(int));

}

#endif