/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcInvalidPacketException.hpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 10:52:25 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/17 01:29:04 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __IRC_INVALID_PACKET_EXCEPTION_HPP__
#define __IRC_INVALID_PACKET_EXCEPTION_HPP__
#include <exception>
#include <string>

class IrcInvalidPacketException : public std::exception
{
	private:
		std::string	_msg;
	
	public:
		explicit IrcInvalidPacketException(std::string const& msg);
		virtual ~IrcInvalidPacketException() throw();
		virtual const char* what() const throw();
};

#endif