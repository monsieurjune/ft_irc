/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcListenBindingException.hpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:17:19 by tponutha          #+#    #+#             */
/*   Updated: 2025/02/01 14:05:13 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __IRC_LISTEN_BINDING_EXCEPTION_HPP__
#define __IRC_LISTEN_BINDING_EXCEPTION_HPP__
#include <exception>
#include <string>

class IrcListenBindingException : public std::exception
{
	private:
		std::string	_msg;

	public:
		IrcListenBindingException(const char *msg);
		virtual ~IrcListenBindingException() throw();
		virtual const char* what() const throw();
};

#endif