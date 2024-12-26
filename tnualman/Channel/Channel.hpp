/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 15:07:49 by tnualman          #+#    #+#             */
/*   Updated: 2024/12/26 19:28:05 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CHANNEL_HPP__
# define __CHANNEL_HPP__

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <ctime>

class Client;

class Channel
{
	// public:

	// 	typedef 
	
	private:

		/** Shamelessly copied from inspIRCd ! :p (including doxy style comments :p)
		 * There, these members are public.
		 * Does this mean inspIRCd does not practice encapsulation??!
		 */

		/** The channel's name.
		 */
		std::string _name;

		/** Time that the object was instantiated (used for TS calculation etc)
		 */
		time_t _timeCreated;

		/** User list. (Tat: Should this be list of client for our ft_irc ?)
		 */
		std::vector<Client> _userList;

		/** Channel topic.
		 * If this is an empty string, no channel topic is set.
		 */
		std::string _topic;

		/** Time topic was set.
		 * If no topic was ever set, this will be equal to Channel::created
		 */
		time_t _timeTopicSet;

		/** The last user to set the topic.
		 * If this member is an empty string, no topic was ever set.
		 */
		std::string _topicSetter;

		Channel(void);

	public:

		Channel(std::string const & name);
		~Channel(void);

		// Orthodox Canonical Form fluff :p
		
		// Channel(Channel const & origin);
		// Channel & operator=(Channel const & rhs);

		// Getters

		std::string			getName(void) const;
		time_t				getTimeCreated(void) const;
		std::vector<Client>	getUserList(void) const;
		int					getUserCount(void) const; // Simply returns the size of the vector from above.
		std::string			getTopic(void) const;
		time_t				getTimeTopicSet(void) const;
		std::string			getTopicSetter(void) const;

};

#endif
