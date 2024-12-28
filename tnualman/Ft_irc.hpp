#ifndef __FT_IRC_HPP__
#define __FT_IRC_HPP__

# ifndef IRC_MAXSIZE
# define IRC_MAXSIZE 512
# endif

# include <poll.h>
# include <vector>
# include <map>
# include <string>

class Client;

class Channel;

class Ft_irc
{
	private:
		std::map<std::string, Channel*>	_channelMap;
		std::map<int, Client*>			_clientMapByFd;
		std::map<std::string, Client*>	_clientMapByUsername;

		std::vector<struct pollfd>		_pollfd_vec;

		std::string						_server_password;

	public:
		Ft_irc(std::string const server_password);
		~Ft_irc(void);
};

#endif