#ifndef __NETWORK_HPP__
#define __NETWORK_HPP__

// Project Header
#include "ft_irc/FtIrcConstants.hpp"

// CPP Header
#include <string>

namespace ft_net
{

int		get_listener_scoket_fd(const char *port_str);
bool	irc_recv(int fd, std::string& msg);

}

#endif