/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcTooLongMsgException.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:01:23 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/17 01:29:28 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __IRC_TOO_LONG_MSG_EXCEPTION_HPP__
#define __IRC_TOO_LONG_MSG_EXCEPTION_HPP__
#include <exception>
#include <string>

class IrcTooLongMsgException : public std::exception
{
	private:
		std::string	_msg;

	public:
		explicit IrcTooLongMsgException(std::string const& msg);
		virtual ~IrcTooLongMsgException() throw();
		virtual const char* what() const throw();
};

#endif