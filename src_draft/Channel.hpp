/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 15:07:49 by tnualman          #+#    #+#             */
/*   Updated: 2025/02/03 18:09:32 by tnualman         ###   ########.fr       */
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
	public:

		typedef std::map<Client*, std::set<char> >	t_userMap;

	private:
	// Attributes
		std::string 		_name;
		std::string			_password;

		std::time_t const	_timeCreated;

		t_userMap			_userMap;
		size_t				_userCountLimit;

		std::set<char> 		_modes;

		std::string			_topic;
		time_t				_timeTopicSet;
		std::string 		_topicSetter;

		std::set<int>		_inviteSet;

	public:
	// De/Constructor
		Channel(std::string const name, Client * const creator);
		~Channel(void);

	// Getters
		std::string const &		getName(void) const;
		std::string const &		getPassword(void) const;
		std::time_t				getTimeCreated(void) const; 
		t_userMap const &		getUserMap(void) const;
		int						getUserCount(void) const;
		int						getUserCountLimit(void) const;
		std::string	const &		getTopic(void) const;
		std::time_t				getTimeTopicSet(void) const;
		std::string	const &		getTopicSetter(void) const;
		std::set<char> const &	getModes(void) const;

	// Checkers
		bool					hasThisClient(Client * const client) const;
		bool					hasThisMode(char const c) const;

	// Setters
		void					setName(std::string const name);
		void					setPassword(std::string const password);
		void					setUserCountLimit(int const limit);
		void					setTopic(std::string const name, Client const * const client);

	// Change channel modes
		void					addMode(char const c);
		void					addMode(std::string s);
		void					removeMode(char const c);
		void					removeMode(std::string s);

	// Add/Delete User
		int						addUserToChannel(Client * const client, std::string modestr);
		int						deleteUserFromChannel(Client * const client);

	// Client membership modes
		std::set<char> const &	getThisClientMembershipModes(Client * const client) const; // Returns a set with only '!' is user is not found in channel.
		bool					hasThisClientMembershipMode(Client * const client, char const c) const; // Returns false if user is not found.
		int						addThisClientMembershipMode(Client * const client, char const c); // Returns -1 is user is not found.
		int						addThisClientMembershipMode(Client * const client, std::string s); // Returns -1 is user is not found.
		int						removeThisClientMembershipMode(Client * const client, char const c); // Returns -1 is user is not found.
		int						removeThisClientMembershipMode(Client * const client, std::string s); // Returns -1 is user is not found.

	// Invitation

		bool					isClientInvited(Client * const client);
		void					addClientToInviteSet(Client * const client);
		void					removeClientToInviteSet(Client * const client);

};

#endif
