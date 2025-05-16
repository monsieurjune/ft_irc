/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcListenBindingException.hpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:17:19 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/17 01:29:17 by tponutha         ###   ########.fr       */
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
		explicit IrcListenBindingException(std::string const& msg);
		virtual ~IrcListenBindingException() throw();
		virtual const char* what() const throw();
};

#endif