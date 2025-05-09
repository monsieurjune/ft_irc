/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CstdException.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:21:11 by tponutha          #+#    #+#             */
/*   Updated: 2025/02/01 14:03:39 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exception/CstdException.hpp"
#include "std/ft_cppstd.hpp"

CstdException::CstdException(int code) : exception()
{
	_msg = ft_std::itoa(code);
}

CstdException::~CstdException() throw() {}

const char *CstdException::what() const throw()
{
	return _msg.c_str();
}