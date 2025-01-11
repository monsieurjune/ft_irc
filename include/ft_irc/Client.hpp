#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

// C Header
#include <ctime>

// CPP Header
#include <string>

class Client
{
	private:
		int		_fd;
		int		_authorizeLevel;
		time_t	_timeConnected;

		std::string	_nickname;
		std::string	_username;
		std::string	_realname;
		std::string	_host;

	public:
		Client(int const fd);
		~Client(void);

		// Getters
		int					getFd(void) const;
		int					getAuthorizeLevel(void) const;
		std::string const&	getNickname(void) const;
		std::string const&	getUsername(void) const;
		std::string const&	getRealname(void) const;
		std::string const&	getHost(void) const;

		// Setters
		void	setAuthorizeLevel(int const level);
		void	setNickname(std::string const name);
		void	setUsername(std::string const name);
		void	setRealname(std::string const realname);
		void	setHost(std::string const str);

};

#endif