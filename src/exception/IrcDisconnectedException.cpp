/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcDisconnectedException.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:21:14 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/17 01:35:06 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exception/IrcDisconnectedException.hpp"

IrcDisconnectedException::IrcDisconnectedException(std::string const& msg) : _msg(msg) {}

IrcDisconnectedException::~IrcDisconnectedException() throw() {}

const char *IrcDisconnectedException::what() const throw() { return _msg.c_str(); }