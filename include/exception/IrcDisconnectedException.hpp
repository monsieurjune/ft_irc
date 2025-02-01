/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcDisconnectedException.hpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 10:52:22 by tponutha          #+#    #+#             */
/*   Updated: 2025/01/30 10:52:22 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __IRC_DISCONNECTED_EXCEPTION_HPP__
#define __IRC_DISCONNECTED_EXCEPTION_HPP__
#include <exception>
#include <string>

class IrcDisconnectedException : public std::exception
{
	private:
		std::string	_msg;

	public:
		IrcDisconnectedException(const char *msg);
		virtual ~IrcDisconnectedException() throw();
		virtual const char* what() const throw();
};

#endif