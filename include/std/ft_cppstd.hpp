/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cppstd.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 05:09:35 by tponutha          #+#    #+#             */
/*   Updated: 2025/01/24 05:14:43 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FT_CPPSTD_HPP__
#define __FT_CPPSTD_HPP__
#include <sys/types.h>
#include <string>

namespace ft_std
{

ssize_t		stoi(std::string str);
size_t		stou(std::string str);
std::string	itoa(ssize_t num);

}

#endif