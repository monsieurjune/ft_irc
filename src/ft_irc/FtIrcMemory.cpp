#include "ft_irc/FtIrcMemory.hpp"
#include <stdexcept>

// TODO: Refactor Here

FtIrcMemory::FtIrcMemory(std::string const password) : _serverPassword(password)
{
	// Save Start Server Time
	std::time(&_timeServerStarted);

	// Initialize CMD Map
	_cmdMap["KICK"] = &ircCommandKICK;
	_cmdMap["INVITE"] = &ircCommandINVITE;
	_cmdMap["MODE"] = &ircCommandMODE;
	_cmdMap["TOPIC"] = &ircCommandTOPIC;
}

// TODO: Write Proper Destructor

FtIrcMemory::~FtIrcMemory(void)
{
	for (std::vector<struct pollfd>::iterator it = _pollfdVec.begin(); it != _pollfdVec.end(); it++)
	{
		it = deleteClient(it->fd);
	}
}

// Getter

Client*	FtIrcMemory::getClientByFd(int const fd) const
{
    try
	{
		return (_clientMapByFd.at(fd));
	}
	catch (std::exception const & e)
	{
		// std::cerr << "Client with socket " << fd << " not found!" << std::endl;
		return (NULL);
	}	
}

Client*	FtIrcMemory::getClientByNickname(std::string const name) const
{
    try
	{
		return (_clientMapByNickname.at(name));
	}
	catch (std::exception const & e)
	{
		// std::cerr << "Client with username " << name << " not found!" << std::endl;
		return (NULL);
	}	
}

Channel*	FtIrcMemory::getChannelByName(std::string const name) const
{
    try
	{
		return (_channelMapByName.at(name));
	}
	catch (std::exception const & e)
	{
		// std::cerr << "Channel named " << name << " not found!" << std::endl;
		return (NULL);
	}	
}

std::string	FtIrcMemory::getServerPassword(void) const
{
    return (_serverPassword);
}

// Setter

/** Returns 0 if client is found and the name is successfully changed;
 * returns -1 if client is not found;
 * returns -2 if the newname alrady exists in the _clientMapByNickname, or if it's the same as the old name.
 */
int	FtIrcMemory::setNickname(std::string const name, std::string const newname)
{
	std::map<std::string, Client*>map = _clientMapByNickname;
	std::map<std::string, Client*>::iterator it = map.find(name);
	
	if (it == map.end())
	{
		// std::cerr << "Client with nickname " << name << " not found!" << std::endl;
		return (-1);
	}
	
	if (name == newname || map.find(newname) != map.end())
	{
		// std::cerr << "Client with nickname " << newname << " already exists!" << std::endl;
		return (-2);
	}
	
	Client* tmp = it->second;
	map.erase(it);
	map[newname] = tmp;
	// map.insert({newname, tmp});

	return (0);
}

int	FtIrcMemory::setChannelName(std::string const name, std::string const newname)
{
	std::map<std::string, Channel*> map = _channelMapByName;
	std::map<std::string, Channel*>::iterator it = map.find(name);
	
	if (it == map.end())
	{
		// std::cerr << "Channel named " << name << " not found!" << std::endl;
		return (-1);
	}
	
	if (name == newname || map.find(newname) != map.end())
	{
		// std::cerr << "Channel named " << newname << " already exists!" << std::endl;
		return (-2);
	}
	
	Channel* tmp = it->second;
	map.erase(it);
	map[newname] = tmp;
	// map.insert({newname, tmp});

	return (0);
}

void	FtIrcMemory::setServerPassword(std::string password)
{
	_serverPassword = password;
}

// TODO: may handle ENOMEM later

void	FtIrcMemory::addClient(int const fd)
{
	try
	{
		Client*	client = new Client(fd);

		_clientMapByFd[fd] = client;
	}
	catch(const std::bad_alloc& e)
	{
	}
}

std::vector<pollfd>::iterator	FtIrcMemory::deleteClient(int const fd)
{
	std::map<int, Client*>::iterator	it1 = _clientMapByFd.find(fd);
	std::vector<pollfd>::iterator		ret_it;

	if (it1 == _clientMapByFd.end())
	{
		return _pollfdVec.end();
	}

	Client*	client = it1->second;

	// Erase This Client from map
	_clientMapByFd.erase(fd);
	_clientMapByNickname.erase(client->getNickname());

	// TODO: Remove Client from channel (there is no method to do that now)

	// Let Deconstructor to close fd
	delete client;

	// Erase Client's fd from pollfd
	for (std::vector<struct pollfd>::iterator it = _pollfdVec.begin(); it != _pollfdVec.end(); it++)
	{
		if (it->fd == fd)
		{
			return _pollfdVec.erase(it);
		}
	}
	return _pollfdVec.end();
}

std::string	FtIrcMemory::ircMessageHandler(Message const & message, Client const * const sender)
{
	try
	{
		FtIrcMemory::IrcFunc	func = _cmdMap.at(message.getCommand());

		return func(message, sender);
	}
	catch (std::out_of_range const& e)
	{
		// TODO: Put log here
		return "";
	}
}