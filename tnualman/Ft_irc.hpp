/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 01:57:58 by tnualman          #+#    #+#             */
/*   Updated: 2024/12/29 02:59:57 by tnualman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FT_IRC_HPP__
# define __FT_IRC_HPP__

# ifndef IRC_MAXSIZE
#  define IRC_MAXSIZE 512
# endif

# include <poll.h>
# include <ctime>
# include <vector>
# include <map>
# include <string>

// TODO: Replace these forward declarations for these classes
// with #include once those classes are done.

class Client;
class Channel;

class Ft_irc
{
	private:
		
		std::map<int, Client*>			_clientMapByFd;
		std::map<std::string, Client*>	_clientMapByUsername;
		std::map<std::string, Channel*>	_channelMapByName;

		std::vector<struct pollfd>		_pollfdVec;

		std::string						_serverPassword;
		time_t							_timeServerStarted;

		Ft_irc(void);

	public:
		
		Ft_irc(std::string const server_password);
		~Ft_irc(void);

		// Getters

		Client*		getClientByFd(int const fd) const;
		Client*		getClientByUsername(std::string const name) const;
		Channel*	getChannelByName(std::string const name) const;
		std::string	getServerPassword(void) const;

		// Changers; will discuss these before implementing...

		// int	changeFd(int const fd, int const newfd); // Probably NOT! :p
		int		changeUsername(std::string const name, std::string newname);
		int		changeChannelName(std::string const name, std::string newname);
		void	changeServerPassword(std::string password);

};

#endif