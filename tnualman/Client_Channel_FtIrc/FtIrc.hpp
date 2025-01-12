/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtIrc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnualman <tnualman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 01:57:58 by tnualman          #+#    #+#             */
/*   Updated: 2024/12/31 20:50:08 by tnualman         ###   ########.fr       */
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
# include <set>
# include <string>
# include <iterator>
# include <iostream>

# include "Client.hpp"
# include "Channel.hpp"
# include "Message.hpp"
# include "e_numerics.hpp"

class FtIrc
{			
	private:
		
		std::set<Client>				_clientSet;	
		std::map<int, Client*>			_clientMapByFd;
		std::map<std::string, Client*>	_clientMapByNickname;
		std::set<Channel>				_channelSet;
		std::map<std::string, Channel*>	_channelMapByName;

		std::vector<struct pollfd>		_pollfdVec;

		std::string						_serverPassword;
		time_t							_timeServerStarted;

		// Hard-coded with fixed array for now, because function pointer's syntax in C++ is harder than I expected...
		int								(FtIrc::*_commandHandler[4])(Message const &, Client * const, std::string &);
		std::vector<std::string>		_availableCommands;

	public:
		
		FtIrc(std::string const server_password);
		~FtIrc(void);

		// Getters
		Client*		getClientByFd(int const fd) const;
		Client*		getClientByNickname(std::string const name) const;
		Channel*	getChannelByName(std::string const name) const;
		std::string	getServerPassword(void) const;

		// Changers
		// int	changeFd(int const fd, int const newfd); // Probably NOT! :p
		int		changeNickname(std::string const name, std::string const newname);
		int		changeChannelName(std::string const name, std::string const newname);
		void	changeServerPassword(std::string password);

		// Adders
		int		addClient(int const fd, std::string const nickname,
					std::string const username, std::string const host, std::string modestr);
		int		addChannel(std::string const name);

		// Deleters
		int		deleteClient(Client * const client);
		int		deleteClient(int const fd);
		int		deleteClient(std::string const nickname);
		int		deleteChannel(Channel * const channel);
		int		deleteChannel(std::string const name);

		// IRC Message handler
		int	ircMessageHandler(Message const & message, Client * const sender, std::string & reply);
		int	ircCommandKICK(Message const & message, Client * const sender, std::string & reply);
		int	ircCommandINVITE(Message const & message, Client * const sender, std::string & reply);
		int	ircCommandMODE(Message const & message, Client * const sender, std::string & reply);
		int	ircCommandTOPIC(Message const & message, Client * const sender, std::string & reply);

		int	ircCommandMODE_channel(Message const & message, Client * const sender, std::string & reply);
		int	ircCommandMODE_user(Message const & message, Client * const sender, std::string & reply);
};

#endif