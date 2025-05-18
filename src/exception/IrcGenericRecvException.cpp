/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcGenericRecvException.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:21:11 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/17 20:13:39 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exception/IrcGenericRecvException.hpp"

IrcGenericRecvException::IrcGenericRecvException(std::string const& msg) : _msg(msg) {}

IrcGenericRecvException::~IrcGenericRecvException() throw() {}

const char *IrcGenericRecvException::what() const throw() { return _msg.c_str(); }