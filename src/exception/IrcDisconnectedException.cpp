/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcDisconnectedException.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:21:14 by tponutha          #+#    #+#             */
/*   Updated: 2025/01/30 18:21:14 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exception/IrcDisconnectedException.hpp"

IrcDisconnectedException::IrcDisconnectedException(const char *msg) : exception()
{
	_msg = std::string(msg);
}

IrcDisconnectedException::~IrcDisconnectedException() throw() {}

const char *IrcDisconnectedException::what() const throw()
{
	return _msg.c_str();
}