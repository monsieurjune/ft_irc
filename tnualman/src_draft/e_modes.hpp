/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_modes.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 13:57:56 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/29 19:59:39 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __E_MODES_HPP__
# define __E_MODES_HPP__

enum e_modes
{
	MODE_INVITEONLY = 'i',
	MODE_CHANNELKEY = 'k',
	MODE_USERLIMIT = 'l',
	MODE_OPERATOR = 'o',
	MODE_PROTECTTOPIC = 't'
};

#endif