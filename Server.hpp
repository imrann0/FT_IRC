#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <netinet/in.h>	// sockamddr_in
#include <poll.h>		//poll
#include <vector>		//vector
#include <map>			//map
#include "Client.hpp"
#include "Channel.hpp"

#define RPL_JOIN(source, channel) ":" + source + " JOIN :" + channel

class Server
{
	public:
		Server(int port, std::string password);
		void	Debug();
		void	acceptClient();
		void	handleClientEvents();
		void	processUserEvents();
		void	processMessage(int clientFd, const char* buffer, std::map<int, Client>& clients);
	private:
		std::string serverName;
		const int _port;
		const std::string _password;
		int _socket;
		struct sockaddr_in server_addr;
		std::vector<pollfd> _pollFds;
		std::map<int, Client> _clients;
		std::map<std::string, Channel> channels;
};


#endif
