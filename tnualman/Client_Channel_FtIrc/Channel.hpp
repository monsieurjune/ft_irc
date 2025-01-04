/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 15:07:49 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/03 20:16:02 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CHANNEL_HPP__
# define __CHANNEL_HPP__

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <bitset>
# include <ctime>

# include "Client.hpp"

class Channel
{
	public:
		
		typedef std::bitset<4> t_userFlags;
		typedef std::bitset<4> t_channelFlags;

	private:

		/** Shamelessly copied from inspIRCd ! :p (including doxy style comments :p)
		 * There, these members are public.
		 * Does this mean inspIRCd does not practice encapsulation??!
		 */

		/** The channel's name.
		 */
		std::string _name;

		/** Time that the object was i(Might not be used)nstantiated (used for TS calculation etc)
		 * (Might not be used)
		 */
		time_t _timeCreated;

		/** User map. associated int value is for user-channel membership flags/statuses.
		 */
		std::map<Client*, t_userFlags> _userMap;

		/** Channel topic.
		 * If this is an empty string, no channel topic is set.
		 */
		std::string _topic;

		long		_user_limit;

		/** Time topic was set.
		 * If no topic was ever set, this will be equal to Channel::created
		 */
		time_t _timeTopicSet;

		/** The last user to set the topic.
		 * If this member is an empty string, no topic was ever set.
		 */
		std::string _topicSetter;

		/** Channel's password.
		 */
		std::string	_password;

		/** Channel's mode flags
		 */
		t_channelFlags _channelFlags;

		Channel(void);

	public:

		Channel(std::string const name);
		~Channel(void);

		// Orthodox Canonical Form fluff :p
		
		// Channel(Channel const & origin);
		// Channel & operator=(Channel const & rhs);

		// Getters

		std::string const &	getName(void) const;
		time_t				getTimeCreated(void) const;
		t_userFlags			getUserFlags(Client * const client) const;
		int					getUserCount(void) const; // Simply returns the size of the map from above.
		std::string	const &	getTopic(void) const;
		time_t				getTimeTopicSet(void) const;
		std::string	const &	getTopicSetter(void) const;

		// Setters
		void			setName(std::string const name);
		
		/** status MUST be positive, because I define negative as user/client not found.
		 * Returns status or -1 if user/client is not found.
		 */
		t_userFlags		setUserStatus(Client * const client, Channel::t_userFlags const & flags);
		void			setTopic(std::string const name, Client const * const client);

		// Adder
		int		addUserToChannel(Client * const client, int const status);
};

#endif