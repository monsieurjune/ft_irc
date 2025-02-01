/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcUnknownCmdException.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 10:52:28 by tponutha          #+#    #+#             */
/*   Updated: 2025/01/30 10:52:28 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __IRC_UNKNOWN_CMD_EXCEPTION_HPP__
#define __IRC_UNKNOWN_CMD_EXCEPTION_HPP__
#include <exception>
#include <string>

class IrcUnknownCmdException : public std::exception
{
	private:
		std::string	_msg;

	public:
		IrcUnknownCmdException(const char *msg);
		virtual ~IrcUnknownCmdException() throw();
		virtual const char* what() const throw();
};

#endif