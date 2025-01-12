/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc_ircCommandMODE.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 22:04:23 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/12 22:04:54 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FtIrc.hpp"

int FtIrc::ircCommandMODE(Message const & message, Client * const sender, std::string & reply)
{
	std::vector<std::string> const & params = message.getParams();

	if (params.size() == 0)
	{
		output = "No parameters given for MODE command!";
		// Find the correct return code from e_numerics.hpp .
		return (-2);
	}

	try
	{
		if (params.at(0).at(0) == '#')
		{
			return (ircCommandMODE_channel(message, sender, output));
		}
		else
		{
			return (ircCommandMODE_user(message, sender, output));
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << "Message parsing error: empty string for <target>!" << std::endl;
		output = "No parameters given for MODE command!";
		// Find the correct return code from e_numerics.hpp .
		return (-2);
	}
}