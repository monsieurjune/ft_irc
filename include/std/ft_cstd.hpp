/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cstd.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 05:09:39 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/19 06:38:34 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FT_CSTD_HPP__
#define __FT_CSTD_HPP__
#include <sys/types.h>
#include <cstddef>

namespace ft_std
{

void	*memset(void *ptr, int c, size_t byte);
bool	isnumber(const char *num);
ssize_t	strcnlen(const char *str, int c, ssize_t n);
char	*strnstr(const char *str, const char *find, size_t n);
int     log10i(ssize_t num);

}

#endif