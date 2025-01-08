#ifndef __FT_IRC_MEMORY_HPP__
#define __FT_IRC_MEMORY_HPP__

// Project Header
#include "Client.hpp"
#include "Channel.hpp"
#include "Message.hpp"

// C Header
#include <poll.h>

// C Header
# include <ctime>

// CPP Header
#include <map>
#include <string>
#include <vector>

class FtIrcMemory
{
	private:
		typedef std::string (*_IrcFunc)(Message const &, Client const * const);

		// Map-for-Client-Access Attribute
		std::map<int, Client*>			_clientMapByFd;
		std::map<std::string, Client*>	_clientMapByNickname;
		std::map<std::string, Channel*>	_channelMapByName;

		// Poll-FD Attribute
		std::vector<struct pollfd>		_pollfdVec;

		// Server-Unique Attribute
		std::string						_serverName;
		std::string						_serverPassword;
		time_t							_timeServerStarted;

		// Map-of-IRC-CMD Attribute
		std::map<std::string, _IrcFunc>	_cmdMap;

		// Create/Delete-Channel Method
		int	addChannel(Channel * const channel);
		int	deleteChannel(Channel * const channel);

		// IRC-CMD Method
		std::string	ircCommandKICK(Message const & message, Client const * const sender);
		std::string	ircCommandINVITE(Message const & message, Client const * const sender);
		std::string	ircCommandMODE(Message const & message, Client const * const sender);
		std::string	ircCommandTOPIC(Message const & message, Client const * const sender);

	public:
		FtIrcMemory(std::string const server_password);
		~FtIrcMemory(void);

		// Getters
		Client*		getClientByFd(int const fd) const;
		Client*		getClientByUsername(std::string const name) const;
		Channel*	getChannelByName(std::string const name) const;
		std::string	getServerPassword(void) const;

		// Setter
		int		setNickname(std::string const name, std::string newname);
		int		setChannelName(std::string const name, std::string newname);
		void	setServerPassword(std::string const password);

		// Connect/Disconect
		void	addClient(int const fd);
		void	deleteClient(int const fd);

		// IRC Message Handler
		std::string	ircMessageHandler(Message const & message, Client const * const sender);
};

#endif