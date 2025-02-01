/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_init.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 14:46:07 by tponutha          #+#    #+#             */
/*   Updated: 2025/01/31 16:55:04 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Header
#include "utils/ft_utils.hpp"

// C Header
#include <signal.h>

namespace ft_utils
{

int	signal_init(void (*killer)(int), void (*ignorer)(int))
{
	if (signal(SIGINT, killer) == SIG_ERR)
	{
		return -1;
	}

	if (signal(SIGPIPE, ignorer) == SIG_ERR)
	{
		return -1;
	}

	return 0;
}

}