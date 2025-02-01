/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcInvalidPacketException.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:21:18 by tponutha          #+#    #+#             */
/*   Updated: 2025/02/01 14:03:44 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exception/IrcInvalidPacketException.hpp"

IrcInvalidPacketException::IrcInvalidPacketException(const char *msg) : exception()
{
	_msg = std::string(msg);
}

IrcInvalidPacketException::~IrcInvalidPacketException() throw() {}

const char *IrcInvalidPacketException::what() const throw()
{
	return _msg.c_str();
}