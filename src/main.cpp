#include "network/network.hpp"
#include "utils/ft_utils.hpp"

#include <unistd.h>

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

	if (ft_utils::set_non_blocking(fd) < 0)
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

int	main(const int argc, char *argv[])
{
	std::vector<struct pollfd>	pollfd_vector(1024);
	int							listen_socketfd;
	int							poll_count				= 0;
	int							poll_check_count		= 0;
	char						recv_buff[512 + 1];
	// struct sockaddr_storage

	if (argc != 3)
	{
		// TODO: Fix log here
		std::cerr << "[ERROR]: You must provide exactly 2 argument (port, password)" << std::endl;
		return 1;
	}

	// Create Listen Socket
	listen_socketfd = ft_net::get_listener_scoket_fd(argv[1]);
	if (listen_socketfd < 0)
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
					long re = recv(pollfd_vector[i].fd, recv_buff, 512, 0);

					if (re <= 0)
					{
						std::cout << "Someone Unexceptly Disconnect with code: " << re << std::endl;
						sb_delete_pollfd(pollfd_vector, pollfd_vector[i].fd);
					}
					else
					{
						recv_buff[re] = 0;

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