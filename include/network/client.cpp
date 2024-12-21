#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__
#include <string>

namespace ft_net
{

class Client
{
	public:
		Client(int fd);
		~Client();

		int 	getFd();
		// void	setAuthorizeLevel(std::string& name);
		int 	getAuthorizeLevel();
	
	private:
		int	_authorize_level;
		int	_fd;
};

}

#endif