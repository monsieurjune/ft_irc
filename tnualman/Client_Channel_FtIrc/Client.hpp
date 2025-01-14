/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 00:23:19 by tnualman          #+#    #+#             */
/*   Updated: 2025/01/11 20:21:25 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

# include <string>
# include <ctime>
# include <set>

# include "e_numerics.hpp"

class Client
{
	private:
		
		int				_fd;
		int				_authorizeLevel;
		time_t			_timeConnected; // Does this happen at construction time?

		std::string		_nickname;
		std::string		_username;
		std::string		_host;

		std::set<char>	_modes;

	public:
		
		Client(int const fd);
		Client(int const fd, std::string const nickname,
				std::string const username, std::string const host, std::string modestr);
		~Client(void);

		// Orthodox Canonical Form fluff :p
		
		// Client(Client const & origin);
		// Client & operator=(Client const & rhs);

		// Getters
		
		int	getFd(void) const;
		int	getAuthorizeLevel(void) const;

		std::string const &		getNickname(void) const;
		std::string	const &		getUsername(void) const;
		std::string	const & 	getHost(void) const;

		std::set<char> const &	getModes(void) const;
		bool					hasMode(char c) const;

		// Setters
		
		// void	setFd(int fd); // I'm pretty sure this is not how it works... :p
		void	setAuthorizeLevel(int const level);
		void	setNickname(std::string const name);
		void	setUsername(std::string const name);
		void	setHost(std::string const str);

		void	addMode(char c);
		void	removeMode(char c);
		void	addMode(std::string s);
		void	removeMode(std::string s);

};

#endif