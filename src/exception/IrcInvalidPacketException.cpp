/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcInvalidPacketException.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:21:18 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/17 01:31:04 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exception/IrcInvalidPacketException.hpp"

IrcInvalidPacketException::IrcInvalidPacketException(std::string const& msg) : _msg(msg) {}

IrcInvalidPacketException::~IrcInvalidPacketException() throw() {}

const char *IrcInvalidPacketException::what() const throw() { return _msg.c_str(); }