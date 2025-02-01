/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcListenBindingException.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:21:23 by tponutha          #+#    #+#             */
/*   Updated: 2025/01/30 20:11:53 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exception/IrcListenBindingException.hpp"

IrcListenBindingException::IrcListenBindingException(const char *msg) : exception()
{
	_msg = msg;
}

IrcListenBindingException::~IrcListenBindingException() throw() {}

const char	*IrcListenBindingException::what() const throw()
{
	return _msg.c_str();
}