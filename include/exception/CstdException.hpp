/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CstdException.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 10:52:19 by tponutha          #+#    #+#             */
/*   Updated: 2025/01/30 10:52:20 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CSTD_EXCEPTION_HPP__
#define __CSTD_EXCEPTION_HPP__
#include <exception>
#include <string>

class CstdException : public std::exception
{
	private:
		std::string	_msg;

	public:
		CstdException(int code);
		virtual ~CstdException() throw();
		virtual const char* what() const throw();
};

#endif