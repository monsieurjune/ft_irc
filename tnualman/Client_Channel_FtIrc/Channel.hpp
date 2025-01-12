/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 15:07:49 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/12 21:03:10 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CHANNEL_HPP__
# define __CHANNEL_HPP__

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <set>
# include <ctime>

# include "Client.hpp"
# include "e_numerics.hpp"

class Channel
{
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

		/** User map. associated ser of chars is user membership modes.
		 */
		std::map<Client*, std::set<char>> _userMap;

		/** User count limit; no limit if value is negative.
		*/
		int _userCountLimit;
		
		/** Channel's password.
		 */
		std::string	_password;

		/** Channel's mode flags, represented as a set of chars, for making it work with char and string function arguments.
		 */
		std::set<char> _modes;
		
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

	public:

		Channel(std::string const name);
		~Channel(void);

		// Orthodox Canonical Form fluff :p
		
		// Channel(Channel const & origin);
		// Channel & operator=(Channel const & rhs);

		// Getters
		std::string const &		getName(void) const;
		time_t					getTimeCreated(void) const; 
		int						getUserCount(void) const;
		int						getUserCountLimit(void) const;
		bool					hasUser(Client * const client) const;
		std::string	const &		getTopic(void) const;
		time_t					getTimeTopicSet(void) const;
		std::string	const &		getTopicSetter(void) const;
		
		std::set<char> const &	getModes(void) const;
		bool					hasMode(char const c) const;

		// Setters
		void	setName(std::string const name);
		void	setUserCountLimit(int const limit);
		void	setTopic(std::string const name, Client const * const client);
		
		void	addMode(char const c);
		void	removeMode(char const c);
		void	addMode(std::string s);
		void	removeMode(std::string s);

		// Adder
		int		addUserToChannel(Client * const client, std::string modestr);
		
		// Deleter
		int		deleteUserFromChannel(Client * const client);

		// Membership modes
		std::set<char> const &	getMembershipModes(Client * const client) const; // Returns a set with only '!' is user is not found in channel.
		bool					hasMembershipMode(Client * const client, char const c) const; // Returns false if user is not found.
		char					addMembershipMode(Client * const client, char const c); // Returns NULL is user is not found in channel.
		char					removeMembershipMode(Client * const client, char const c); // Returns NULL is user is not found in channel.
		int						addMembershipMode(Client * const client, std::string s);
		int						removeMembershipMode(Client * const client, std::string s);

};

#endif