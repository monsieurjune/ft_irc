/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcContinueException.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:38:03 by tponutha          #+#    #+#             */
/*   Updated: 2025/01/31 11:39:34 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exception/IrcContinueException.hpp"

IrcContinueException::IrcContinueException(const char *msg): exception()
{
	_msg = msg;
}

IrcContinueException::~IrcContinueException() throw() {}

const char	*IrcContinueException::what() const throw()
{
	return _msg.c_str();
}