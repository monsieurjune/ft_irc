/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc_ircCommandMODE.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 22:04:23 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/14 20:12:52 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FtIrc.hpp"

int FtIrc::ircCommandMODE(Message const & message, Client * const sender)
{
	std::vector<std::string> const & params = message.getParams();

	if (params.size() == 0)
	{
		_replies.push_back("No parameters given for MODE command!");
		// Find the correct return code from e_numerics.hpp .
		return (-2);
	}

	try
	{
		if (params.at(0).at(0) == '#')
		{
			return (ircCommandMODE_channel(message, sender));
		}
		else
		{
			return (ircCommandMODE_user(message, sender));
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << "Message parsing error: empty string for <target>!" << std::endl;
		_replies.push_back("No parameters given for MODE command!");
		// Find the correct return code from e_numerics.hpp .
		return (-2);
	}
}

int FtIrc::ircCommandMODE_channel(Message const & message, Client * const sender)
{
	std::vector<std::string> const & params = message.getParams();
	std::string const target = params.at(0).substr(1, std::string::npos);
	Channel * const channel = getChannelByName(target);
	
	if (channel == NULL)
	{
		//_replies.push_back(":" + _serverName + " :" + sender->getNickname() + " " + target + " : No such channel"); 
		return (ERR_NOSUCHCHANNEL);
	}

	if (params.size() == 1)
	{
		_replies.push_back("");
		return (0);
	}
	return (0);
}