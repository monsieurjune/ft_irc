/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 17:26:28 by tnualman          #+#    #+#             */
/*   Updated: 2024/12/25 15:56:21 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MESSAGE_HPP__
# define __MESSAGE_HPP__

# include <string>
# include <vector>
# include <map>

class Message
{
	private:
		// std::string							_raw; // Memory usage is at premium... :p
		// std::map<std::string, std::string>	_tags; // Not to be implemented (?)
		std::string							_source; // a.k.a. prefix 
		std::string							_command;
		std::vector<std::string>			_params;
		bool								_is_valid;

		Message(void);
	
	public:
		Message(std::string const raw);
		
		~Message(void);
		
		// Orthodox Canonical Form fluff :p
		// Message(Message const & origin);
		// Message & operator=(Message const & rhs);

		// Returns status value, implemented separately from the constructor for the sake of modularity.
		int	parse(std::string const raw);  

		// Getters
		// std::string 						getRawMessage(void) const;
		// std::map<std::string, std::string>	getTags(void) const; // Not to be implemented (?)
		std::string							getSource(void) const;
		std::string							getCommand(void) const;
		std::vector<std::string>			getParams(void) const;
		bool								isValid(void) const;

		// Setters
		// char *							setRawMessage(void) const;
};

#endif