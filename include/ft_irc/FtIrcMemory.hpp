#ifndef __FT_IRC_MEMORY_HPP__
#define __FT_IRC_MEMORY_HPP__

// C Header
#include <poll.h>

// CPP Header
#include <map>
#include <string>
#include <vector>

class Client;
class Channel;

class FtIrcMemory
{
	private:
		std::map<int, Client*>			_clientMapByFd;
		std::map<std::string, Client*>	_clientMapByUsername;
		std::map<std::string, Channel*>	_channelMapByName;

		std::vector<struct pollfd>		_pollfdVec;

		std::string						_serverPassword;

		FtIrcMemory(void);
	
	public:
		FtIrcMemory(std::string const server_password);
		~FtIrcMemory(void);

		// Getters
		Client*		getClientByFd(int const fd) const;
		Client*		getClientByUsername(std::string const name) const;
		Channel*	getChannelByName(std::string const name) const;
		std::string	getServerPassword(void) const;

		// Setter
		int		setUsername(std::string const name, std::string newname);
		int		setChannelName(std::string const name, std::string newname);
		void	setServerPassword(std::string password);

		// Connect/Disconect
		void	addClient();
		void	deleteClient();

};

#endif