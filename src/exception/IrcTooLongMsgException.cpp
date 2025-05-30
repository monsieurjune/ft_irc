/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcTooLongMsgException.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:54:54 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/17 01:37:21 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exception/IrcTooLongMsgException.hpp"

IrcTooLongMsgException::IrcTooLongMsgException(std::string const& msg) : _msg(msg) {}

IrcTooLongMsgException::~IrcTooLongMsgException() throw() {}

const char	*IrcTooLongMsgException::what() const throw() { return _msg.c_str(); }