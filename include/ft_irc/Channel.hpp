/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tponutha <tponutha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 02:56:42 by tponutha          #+#    #+#             */
/*   Updated: 2025/01/22 05:53:31 by tponutha         ###   ########.fr       */
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
		 * @brief Maximum User in This Channel. Ignore if userlimit mode is unset
		 */
		size_t	_userCountLimit;

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

	public:
		/**
		 * @brief Construct an Channel object
		 * 
		 * @param name Channel's Name
		 * @param creator Client that requested to create channel
		 */
		Channel(std::string const name, Client * const creator);
		
		/**
		 * @brief Default Destructor
		 * 
		 * @note 
		 * Should pop out this channel from client's channel set.
		 * Despite the fact that, it must have 0 user to be deleted
		 */
		~Channel();

		/**
		 * @brief Get Channel's Name
		 * 
		 * @return Channel's Name in rvalue
		 */
		std::string const&	getName()	const;

		/**
		 * @brief Get Time when this channel is created
		 * 
		 * @return Time when this channel is created
		 */
		time_t	getTimeCreated()	const;

		/**
		 * @brief Get Amount of Client that joined this Channel
		 * 
		 * @return Amount of Client that stored in map
		 */
		size_t	getUserCount()	const;

		/**
		 * @brief Get Amount of User Limit
		 * 
		 * @return If userlimit flag is setted, return N, otherwise return 0
		 */
		size_t	getUserCountLimit()	const;

		/**
		 * @brief Get This Channel Topic's Name
		 * 
		 * @return Topic Name in rvalue
		 */
		std::string const&	getTopic()	const;

		/**
		 * @brief Get Most Recent Time that topic is setted
		 * 
		 * @return Most Recent Time that topic is setted
		 */
		time_t	getTimeTopicSet()	const;

		/**
		 * @brief Get Most Recent Topic Setter's Nickname
		 * 
		 * @return Nickname of Most Recent Topic Setter in rvalue
		 */
		std::string const&	getTopicSetter()	const;

		/**
		 * @brief Get Channel Modes 
		 * 
		 * @note not to be confused with client's membership modes
		 * 
		 * @return Channel Mode Set in rvalue
		 */
		std::set<char> const&	getModes()	const;
};

#endif