/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_modes.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 13:57:56 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/28 14:05:20 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __E_MODES_HPP__
# define __E_MODES_HPP__

enum e_modes
{
	MODE_INVITEONLY = 'i',
	MODE_PROTECTTOPIC = 't',
	MODE_CHANNELKEY = 'k',
	MODE_OPERATOR = 'o',
	MODE_USERLIMIT = 'l',
};

#endif