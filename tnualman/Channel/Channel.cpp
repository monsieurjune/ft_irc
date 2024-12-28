/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 17:40:40 by tnualman          #+#    #+#             */
/*   Updated: 2024/12/28 13:06:04 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(void) {}

Channel::Channel(std::string const & name): _name(name)
{
    // std::time(&_timeCreated);
    std::time(&_timeTopicSet);
}

Channel::~Channel(void) {}