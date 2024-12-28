#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

# include <string>

class Client
{
	private:
		
		int	_fd;
		int	_authorize_level;

		std::string	_nickname;
		std::string	_username;
		std::string	_host;

		Client(void);

	public:
		Client(int fd);
		Client(int fd, std::string const _nickname, std::string const _username, std::string const _host);
		~Client(void);

		int 	getFd(void);
		// void	setAuthorizeLevel(std::string& name);
		int 	getAuthorizeLevel(void);
};

#endif