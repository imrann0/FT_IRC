#ifndef SERVER_HPP
# define SERVER_HPP

#include <netinet/in.h>	// sockamddr_in
#include <poll.h>		//poll
#include <vector>		//vector
#include <map>			//map
#include "Client.hpp"
#include "Channel.hpp"

class Server
{
	public:
		Server(int port, std::string password);
		~Server();
		void	Debug();
		void	acceptClient();
		void	processUserEvents();
		void	processMessage(Client &client);
		void	routeCommand(Client &Client, std::vector<std::string> &cmd);
		void	login(Client &client, std::vector<std::string>	&str);
		void	list(Client &client);
		int		receiveData(Client &client);
		static void setSignal(int signal);

		std::string	getPassword() const;
	private:
		std::vector<pollfd>				_pollFds;
		std::map<int, Client>			_clients;
		std::string						serverName;
		const int						_port;
		const std::string				_password;
		int								_socket;
		struct sockaddr_in				server_addr;
		std::map<std::string, Channel>	_channels;
		static int						_signal;
};


#endif
