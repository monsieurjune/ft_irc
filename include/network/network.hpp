#ifndef __NETWORK_HPP__
#define __NETWORK_HPP__
#include <string>

#ifndef IRC_MAXSIZE
#define IRC_MAXSIZE 512
#endif

#ifndef IRC_TERMINATE
#define IRC_TERMINATE "\r\n"
#endif

namespace ft_net
{

int		get_listener_scoket_fd(const char *port_str);
bool	irc_recv(int fd, std::string& msg);

}

#endif