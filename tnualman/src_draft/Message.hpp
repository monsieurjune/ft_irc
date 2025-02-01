/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 17:26:28 by tnualman          #+#    #+#             */
/*   Updated: 2025/02/01 21:18:58 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MESSAGE_HPP__
# define __MESSAGE_HPP__

# include <string>
# include <sstream>
# include <vector>
# include <map>

# include "e_numerics.hpp"

class Message
{
	private:
	// Attributes
		std::string					_source; // a.k.a. prefix
		std::string					_command;
		std::vector<std::string>	_params;
		bool						_isValid;
	
	public:
	// De/Constructor
		Message(void);
		Message(std::string const msg);
		~Message(void);

	// Copy-assignment
		Message(Message const & origin);
		Message & operator=(Message const & rhs); 
		
	private:
	// Parser
		int	parse(std::string const raw);  

	public:
	// Getters
		std::string	const &					getSource(void) const;
		std::string	const &					getCommand(void) const;
		std::vector<std::string> const &	getParams(void) const;
		bool								isValid(void) const;
		std::string const &					getMessage(void);

	// Setters
		void								setSource(std::string src);
		void								setCommand(std::string cmd);
		void								setCommand(e_numerics cmd);
		void								resetParams(void);
		void								setParams(std::vector<std::string> const & params);
		void								concatParams(std::vector<std::string> const & params);
		void								pushParam(std::string param);
	
};

#endif
