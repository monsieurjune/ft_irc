/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcContinueException.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:33:57 by tponutha          #+#    #+#             */
/*   Updated: 2025/01/31 11:36:27 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __IRC_CONTINUE_EXCEPTION_HPP__
#define __IRC_CONTINUE_EXCEPTION_HPP__
#include <exception>
#include <string>

class IrcContinueException : public std::exception
{
	private:
		std::string	_msg;

	public:
		IrcContinueException(const char *msg);
		virtual ~IrcContinueException() throw();
		virtual const char* what() const throw();
};

#endif