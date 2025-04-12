/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 21:58:31 by tponutha          #+#    #+#             */
/*   Updated: 2025/04/13 05:59:16 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

// Project Header
#include "FtIrcConstants.hpp"

// CPP Header
#include <string>
#include <vector>
#include <map>

class Message
{
	private:
		/**
		 * @brief IRC Messgae Prefix
		 * 
		 * @note It's optional for client side but mandatory for server side
		 */
		std::string	_source;

		/**
		 * @brief IRC Message Command/Response Code
		 */
		std::string	_command;

		/**
		 * @brief Vector of IRC Message Params
		 * 
		 * @note Last IRC Param should has : as prefix to tell that it's trailing param
		 */
		std::vector<std::string>	_params;

		/**
		 * @brief Validation of IRC Message
		 * 
		 * @note It's only matter when parsing string to Message
		 */
		bool	_isValid;

		/**
		 * @brief Indicator that this message contain trailing parameter (default is false)
		 * 
		 * @note This attribute isn't used when assemble msg
		 */
		bool	_hasTrailing;

		/**
		 * @brief Parser of Raw IRC Message
		 * 
		 * @param raw Raw IRC Message
		 * 
		 * @return
		 * 
		 * - 0: no error
		 * 
		 * - else: error
		 */
		int	parse(std::string const raw);

	public:
		/**
		 * @brief Default Construct of This Object
		 * 
		 * @note Should be use when try to create response msg to client
		 */
		Message();

		/**
		 * @brief Construct of This Object
		 * 
		 * @param msg String of no IRC terminated (\r\n) Message
		 * 
		 * @note Should be use when try to parse reply msg from client
		 */
		Message(std::string const msg);

		/**
		 * @brief Default Destructor of This Object
		 */
		~Message();

		/**
		 * @brief Copy Constructor of This Object
		 * 
		 * @param origin Origin Message
		 */
		Message(Message const& origin);

		/**
		 * @brief Operator Equal of This Object
		 * 
		 * @param rhs Origin Message
		 */
		Message&	operator=(Message const& rhs);

		/**
		 * @brief Get IRC Prefix
		 * 
		 * @return IRC Prefix (If has one) in lvalue
		 */
		std::string const&	getSource()	const;

		/**
		 * @brief Get IRC Command
		 * 
		 * @return IRC Command/Response Code in lvalue
		 */
		std::string const&	getCommand()	const;

		/**
		 * @brief Get IRC Params
		 * 
		 * @return Vector of IRC Params in lvalue
		 */
		std::vector<std::string> const&	getParams()	const;

		/**
		 * @brief Check if this msg contain trailing
		 * 
		 * @return
		 * 
		 * - true: contain trailing
		 * 
		 * - false: doesn't contain trailing
		 */
		bool	hasTrailing() const;

		/**
		 * @brief Get Validation of IRC Message
		 * 
		 * @return True if Parsing MSG is valid or Use Default COnstructor to create, False Otherwise
		 */
		bool	isValid()	const;

		/**
		 * @brief Create Raw IRC Message
		 * 
		 * @return String of proper IRC Message in lvalue
		 */
		std::string	assembleRawMessage();

		/**
		 * @brief Set IRC Prefix
		 * 
		 * @param source IRC Prefix
		 */
		void	setSource(std::string const source);

		/**
		 * @brief Set IRC Command
		 * 
		 * @param cmd IRC Command
		 */
		void	setCommand(std::string const cmd);

		/**
		 * @brief Set IRC Response Code
		 * 
		 * @param cmd IRC Response Code
		 */
		void	setCommand(e_numerics cmd);

		/**
		 * @brief Reset Params Vector in This Message
		 */
		void	resetParams();

		/**
		 * @brief Add Param at the end of Params Vector
		 * 
		 * @param param IRC Param
		 * 
		 * @warning Do not try to add some character that specify IRC Message
		 */
		void	pushParam(std::string const param);
};

#endif