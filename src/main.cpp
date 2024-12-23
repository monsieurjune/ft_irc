#include "network/ft_irc.hpp"
#include "network/network.hpp"
#include "utils/ft_utils.hpp"

#include <unistd.h>
#include <fcntl.h>

#include <poll.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <string>
#include <cstdlib>
#include <vector>

#include <iostream>

static inline void	sb_close_all_fds(std::vector<struct pollfd>& pollfd_vector)
{
	for (size_t i = 0; i < pollfd_vector.size(); i++)
	{
		close(pollfd_vector[i].fd);
	}
}

static inline void	sb_add_pollfd(std::vector<struct pollfd>& pollfd_vector, int fd, short event)
{
	struct pollfd	new_pollfd;

	ft_utils::memset(&new_pollfd, 0, sizeof(new_pollfd));

	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
	{
		return;
	}
	
	new_pollfd.fd = fd;
	new_pollfd.events = event;

	pollfd_vector.push_back(new_pollfd);
}

static inline void	sb_delete_pollfd(std::vector<struct pollfd>& pollfd_vector, int fd)
{
	for (std::vector<struct pollfd>::iterator it = pollfd_vector.begin(); it != pollfd_vector.end(); it++)
	{
		if (it->fd == fd)
		{
			close(fd);
			pollfd_vector.erase(it);
			return;
		}
	}
}

static inline int	sb_argv_parser(const int argc, const char *argv[], std::string& password)
{
	if (argc != 3)
	{
		// TODO: Fix log here
		return -1;
	}

	if (argv[2][0] == '\0')
	{
		// TODO: Fix log here
		return -1;
	}
	password.assign(argv[2]);

	return ft_net::get_listener_scoket_fd(argv[1]);
}

int	main(const int argc, const char *argv[])
{
	std::string					password;
	std::vector<struct pollfd>	pollfd_vector(1024);
	int							listen_socketfd;
	int							poll_count					= 0;
	int							poll_check_count			= 0;
	char						recv_buff[IRC_MAXSIZE + 1];
	// struct sockaddr_storage

	listen_socketfd = sb_argv_parser(argc, argv, password);
	if (listen_socketfd == -1)
	{
		return 1;
	}

	// Set Up listen socket to pollfd
	sb_add_pollfd(pollfd_vector, listen_socketfd, POLLIN);

	// Infinite Loop
	while (1)
	{
		// If Error, Then Just Continue
		poll_check_count = 0;
		poll_count = poll(pollfd_vector.data(), pollfd_vector.size(), 100);
		if (poll_count < 0)
		{
			continue;
		}

		// Loop Through Whole FD
		for (size_t i = 0; i < pollfd_vector.size(); i++)
		{
			if (poll_check_count == poll_count)
			{
				break;
			}

			// Check for fd that ready to read
			if (pollfd_vector[i].revents & POLLIN)
			{
				poll_check_count++;

				if (pollfd_vector[i].fd == listen_socketfd)
				{
					// It mean that someone want to connect
					int client_sockfd = accept(listen_socketfd, NULL, NULL);

					if (client_sockfd < 0)
					{
						// TODO: Put log here
					}
					else
					{
						sb_add_pollfd(pollfd_vector, client_sockfd, POLLIN);
						
						std::cout << "Someone Connect" << std::endl;
						// TODO: Put log here
					}
				}
				else
				{
					// It mean that client send irc msg
					long recv_len = recv(pollfd_vector[i].fd, recv_buff, 512, 0);

					if (recv_len <= 0)
					{
						std::cout << "Someone Unexceptly Disconnect with code: " << recv_len << std::endl;
						sb_delete_pollfd(pollfd_vector, pollfd_vector[i].fd);
					}
					else
					{
						recv_buff[recv_len] = 0;

						std::cout << "Receive: " << recv_buff << std::endl;
					}
				}
			}

			// Check for fd that "HUNG UP"
			if (pollfd_vector[i].revents & POLLHUP)
			{
				poll_check_count++;

				sb_delete_pollfd(pollfd_vector, pollfd_vector[i].fd);
				std::cout << "Someone Disconnect" << std::endl;
			}
		}
	}

    // close socket
	sb_close_all_fds(pollfd_vector);

	return 0;
}