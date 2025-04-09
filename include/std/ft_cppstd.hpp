/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cppstd.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 05:09:35 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/09 16:44:02 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FT_CPPSTD_HPP__
#define __FT_CPPSTD_HPP__

// C Header
#include <sys/types.h>

// CPP Header
#include <string>
#include <vector>

namespace ft_std
{

ssize_t		stoi(std::string str);
size_t		stou(std::string str);
std::string	itoa(ssize_t num);
std::vector<std::string> split(std::string const & s, std::string const & delimiter);

}

#endif