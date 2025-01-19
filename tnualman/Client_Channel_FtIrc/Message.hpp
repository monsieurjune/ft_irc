/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 17:26:28 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/20 02:31:36 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MESSAGE_HPP__
# define __MESSAGE_HPP__

# include <string>
# include <sstream>
# include <vector>
# include <map>

class Message
{
	private:
		// std::string					_raw;
		std::string					_source; // a.k.a. prefix
		// std::string				_sourceNickname;
		// std::string				_sourceUsername;
		// std::string				_sourceHost;
		std::string					_command;
		std::vector<std::string>	_params;
		bool						_isValid;

		Message(void);
	
	public:
		Message(std::string const raw);
		
		~Message(void);
		
		// Orthodox Canonical Form fluff :p
		// Message(Message const & origin);
		// Message & operator=(Message const & rhs);

		// Returns status value, implemented separately from the constructor for the sake of modularity,
		// in case we add something to the constructor later.
		int	parse(std::string const raw);  

		// Getters
		std::string 				getRawMessage(void) const;
		std::string					getSource(void) const;
		// std::string				getSourceNickname(void) const;
		// std::string				getSourceUsername(void) const;
		// std::string				getSourceHost(void) const;
		std::string					getCommand(void) const;
		std::vector<std::string>	getParams(void) const;
		bool						isValid(void) const;

		// Setters
		void						setSource(std::string src);
		void						setCommand(std::string cmd);
		void						setCommand(int cmd);
		void						resetParams(void);
		void						pushParam(std::string param);
		std::string const &			assembleMessage(void);
};

#endif