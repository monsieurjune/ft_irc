/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcUnknownCmdException.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:21:27 by tponutha          #+#    #+#             */
/*   Updated: 2025/02/01 14:03:47 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exception/IrcUnknownCmdException.hpp"

IrcUnknownCmdException::IrcUnknownCmdException(const char *msg) : exception()
{
	_msg = std::string(msg);
}

IrcUnknownCmdException::~IrcUnknownCmdException() throw() {}

const char *IrcUnknownCmdException::what() const throw()
{
	return _msg.c_str();
}