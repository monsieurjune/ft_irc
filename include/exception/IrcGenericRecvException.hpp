/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcGenericRecvException.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 10:52:19 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/17 20:13:09 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CSTD_EXCEPTION_HPP__
#define __CSTD_EXCEPTION_HPP__
#include <exception>
#include <string>

class IrcGenericRecvException : public std::exception
{
	private:
		std::string	_msg;

	public:
		explicit IrcGenericRecvException(std::string const& msg);
		virtual ~IrcGenericRecvException() throw();
		virtual const char* what() const throw();
};

#endif