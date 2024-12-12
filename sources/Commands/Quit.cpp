#include <map>
#include <vector>
#include <unistd.h>		// close
#include <poll.h>
#include "Client.hpp"
#include "Tools.hpp"
#include <stdexcept>
#include <sys/socket.h> // send
#include <iostream>

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


void Quit(std::map<std::string, Channel> &channels, std::map<int, Client>& clients, int clientFd, std::vector<pollfd>& pollFds)
{
	try
	{
		Client &client = clients[clientFd];
		client.MsgToClient("QUIT : Good bye.");
		std::cout << clientFd << " Disconnected() :( " << std::endl;
		RemoveChannels(channels, client);
		clients.erase(clientFd);
		pollFds.erase(Find(pollFds, clientFd));
		close(clientFd);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
