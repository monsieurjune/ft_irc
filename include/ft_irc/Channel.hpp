/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 02:56:42 by tponutha          #+#    #+#             */
/*   Updated: 2025/01/22 03:56:57 by tponutha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

// C Header
#include <ctime>

// Cpp Header
#include <string>
#include <map>
#include <set>

// Forward Decalration to avoid CIRCULAR include
class Client;

class Channel
{
	private:
		/**
		 * @brief Channel's Name
		 * 
		 * @note It must be set at Constructor, and mustn't change
		 */
		std::string	_name;

		/**
		 * @brief Channel's Password
		 * 
		 * @note It's ignorable, when password mode is unset
		 */
		std::string	_password;

		/**
		 * @brief Time when This Channel is created
		 * 
		 * @note It must be set at Constructor, and mustn't change
		 */
		time_t	_timeCreated;

		/**
		 * @brief Map that contain users and its membership mode
		 * 
		 * @note Only insert/erase Client in specific methods
		 */
		std::map<Client *, std::set<char> >	_userMap;

		/**
		 * @brief Channel's Modes
		 */
		std::set<char>	_modes;

		/**
		 * @brief Channel's Topic
		 */
		std::string	_topic;

		/**
		 * @brief The most recent time when topic is setted
		 */
		time_t	_timeTopicSet;

		/**
		 * @brief The Nickname of most recent user that set topic
		 */
		std::string	_topicSetter;
};

#endif