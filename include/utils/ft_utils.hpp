/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:01:25 by tponutha          #+#    #+#             */
/*   Updated: 2025/05/18 10:33:14 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FT_UTILS_HPP__
#define __FT_UTILS_HPP__

// CPP Header
#include <string>

// ANSI color codes
#ifndef COLOR_RESET
#define COLOR_RESET     "\033[0m"
#endif

#ifndef COLOR_BOLD
#define COLOR_BOLD      "\033[1m"
#endif

#ifndef COLOR_BLACK
#define COLOR_BLACK     "\033[30m"
#endif

#ifndef COLOR_RED
#define COLOR_RED       "\033[31m"
#endif

#ifndef COLOR_GREEN
#define COLOR_GREEN     "\033[32m"
#endif

#ifndef COLOR_YELLOW
#define COLOR_YELLOW    "\033[33m"
#endif

#ifndef COLOR_BLUE
#define COLOR_BLUE      "\033[34m"
#endif

#ifndef COLOR_MAGENTA
#define COLOR_MAGENTA   "\033[35m"
#endif

#ifndef COLOR_CYAN
#define COLOR_CYAN      "\033[36m"
#endif

#ifndef COLOR_WHITE
#define COLOR_WHITE     "\033[37m"
#endif

namespace ft_utils
{

typedef enum e_loglevel
{
    DEBUG,
    INFO,
    WARN,
    ERROR,
    CRITICAL
}   t_loglevel;

void	logger(t_loglevel level, std::string const& name, std::string const& msg);
int		signal_init(void (*killer)(int), void (*ignorer)(int));

}

#endif