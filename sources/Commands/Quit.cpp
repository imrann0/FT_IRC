#include <map>
#include <vector>
#include <unistd.h>		// close
#include <poll.h>
#include "Client.hpp"
#include "Command.hpp"
#include <stdexcept>
#include <sys/socket.h> // send

std::vector<pollfd>::iterator Find(std::vector<pollfd>& pollFds, int clientFd)
{
	std::vector<pollfd>::iterator a;
	for (a = pollFds.begin(); a != pollFds.end(); a++)
	{
		if (a->fd == clientFd)
			return (a);
	}
	throw std::runtime_error("Fd Not Found");
}


void Quit(int clientFd, std::map<int, Client>& clients, std::vector<pollfd>& pollFds)
{
	yolla(clientFd, "QUIT : Good bye.\r\n");
	close(clientFd);
	clients.erase(clientFd);
	pollFds.erase(Find(pollFds, clientFd));
}
