/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:13:51 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/18 07:00:38 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "utils/ft_utils.hpp"

// CPP Header
#include <ctime>
#include <string>
#include <iostream>

namespace ft_utils
{

static inline std::string	sb_timestamp()
{
	char		buffer[256];
	std::time_t	now		= std::time(NULL);
	std::tm*	tm_info	= std::localtime(&now);

	// Get format time
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);

	return std::string(buffer);
}

static inline std::string	sb_color_level(t_loglevel level)
{
	std::string	color_str;

	// Get color
	switch (level)
	{
		case DEBUG:
			color_str += "DEBUG";
			break;
		case INFO:
			color_str += COLOR_GREEN;
			color_str += "INFO";
			color_str += COLOR_RESET;
			break;
		case WARN:
			color_str += COLOR_YELLOW;
			color_str += "WARN";
			color_str += COLOR_RESET;
			break;
		case ERROR:
			color_str += COLOR_MAGENTA;
			color_str += "ERROR";
			color_str += COLOR_RESET;
			break;
		case CRITICAL:
			color_str += COLOR_RED;
			color_str += "CRITICAL";
			color_str += COLOR_RESET;
			break;
		default:
			color_str += "DEBUG";
	}

	return color_str;
}

void	logger(t_loglevel level, std::string const& name, std::string const& msg)
{
	std::cout << COLOR_CYAN << sb_timestamp() << COLOR_RESET << " "
				<< sb_color_level(level) << " "
				<< COLOR_BLUE << "[" << name << "]" << COLOR_RESET 
				<< ": " << msg
				<< std::endl;
}

}