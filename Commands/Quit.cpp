#include <map>
#include <vector>
#include <unistd.h>		// close
#include <poll.h>
#include "../Client.hpp"

void Quit(int clientFd, std::map<int, Client>& clients, std::vector<pollfd>& pollFds, size_t index)
{
	close(clientFd);
	clients.erase(clientFd);
	pollFds.erase(pollFds.begin() + index);
}
