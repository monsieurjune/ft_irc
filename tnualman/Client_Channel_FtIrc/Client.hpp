/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 00:23:19 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/22 23:28:05 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

# include <string>
# include <ctime>
# include <set>
# include <queue>

# include "e_numerics.hpp"

class Channel;

class Client
{
	private:
	// Attributes
		int	const				_fd;
		time_t					_timeConnected;
		int						_authenLevel;
		
		std::string				_nickname;
		std::string				_username;
		std::string				_realname;
		std::string				_host;

		std::set<Channel*>		_channels;
		std::set<char>			_modes;

		std::queue<std::string>	_replyQueue;

	public:
	// De/Constructors
		Client(int const fd);
		~Client(void);

	// Getters
		int							getFd(void) const;
		time_t const				getTimeConnected(void) const;
		int							getAuthenLevel(void) const;
		std::string const &			getNickname(void) const;
		std::string	const &			getUsername(void) const;
		std::string	const &			getRealname(void) const;
		std::string	const & 		getHost(void) const;
		std::set<Channel*> const &	getChannels(void) const;
		std::set<char> const &		getModes(void) const;
		bool						hasMode(char c) const;

		bool						isInChannel(Channel * const channel);

	// Setters
		void						setAuthenLevel(int const level);
		void						setNickname(std::string const name);
		void						setUsername(std::string const name);
		void						setRealname(std::string const name);
		void						setHost(std::string const host);
		void						addMode(char c);
		void						removeMode(char c);

	// Edit channel set
		void						editChannelSet(Channel * const channel, bool add);

	// Reply
		size_t						countReply(void);
		void						enqueueReply(std::string const msg);
		std::string	const &			dequeueReply(void);
	
};

#endif