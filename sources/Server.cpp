#include "Server.hpp"
#include "Channel.hpp"
#include "Tools.hpp"
#include "Wordl.hpp"
#include "error.hpp"
#include "Command.hpp"

#include <sys/socket.h> // socket, bind
#include <fcntl.h>		// fcntl, F_SETFL, O_NONBLOCK
#include <cstring>	 	// memset strerror
#include <unistd.h>		// close
#include <stdexcept>	// runtime_error
#include <poll.h>		//poll
#include <string>
#include <cerrno>   // errno
#include <csignal>  // signal
#include <algorithm> // std::error
//								:<server_name> 001 <nick> :Welcome to the Internet Relay Network <nick>!<user>@<host>
#define RPL_WELCOME(serverName, nick, user, host)	":" + serverName + " 001 " + nick + " :Welcome to the Internet Relay Network "+ nick +"!" + user + "@" + host

int Server::_signal = 0;

void signalHandler(int signal)
{
	Server::setSignal(signal);
	usleep(500000);
	usleep(500000);
	usleep(500000);
	usleep(500000);

	std::cout << "Signal: " << signal << std::endl;
}

Server::Server(int port, std::string password) : _port(port), _password(password)
{
	std::cout << "Server Constructor Called" << std::endl;
	serverName  = "irc.myserver.com";
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
		throw std::runtime_error("Socket Error");
	int buffer_size = 1024 * 1024;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &buffer_size, sizeof(buffer_size)) == -1)
	{
		close(_socket);
		throw std::runtime_error("setsockopt Error");
	}
	// non-blockhing
	if (fcntl(_socket, F_SETFL, O_NONBLOCK) == -1)
	{
		close(_socket);
		throw std::runtime_error("fcntl Error");
	}

	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;				 // IPv4 adres ailesi
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // Herhangi bir IP adresi
	server_addr.sin_port = htons(_port);			 // Port numarası

	if (bind(_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
	{
		close(_socket);
		throw std::runtime_error("Bind Error");
	}

	std::cout << "Bind başarılı. Sunucu " << _port << " portuna bağlandı.\n";

	if (listen(_socket, 2) == -1)
	{
		close(_socket);
		throw std::runtime_error("Listen Error");
	}
	struct pollfd fds;
	fds.fd = _socket;
	fds.events = POLLIN;
	fds.revents = 0;
	_pollFds.push_back(fds);
}

Server::~Server()
{
	std::vector<pollfd>::iterator it = _pollFds.begin();
	while (it != _pollFds.end())
	{
		close(it->fd);
		it++;
	}
	std::cout << "Server Destructor Called" << std::endl;
	close(_socket);
}

void  Server::setSignal(int signal)
{
	_signal = signal;
}


void	Server::Debug()
{
	signal(SIGINT, signalHandler);
	while (_signal == 0)
	{
		int ret = poll(&_pollFds[0], _pollFds.size(), -1);
		if (ret == -1)
		{
			close(_socket);
			if (errno == EINTR)
				return ;
			std::cout << strerror(errno) << std::endl;
			throw std::runtime_error("poll Error");
		}
		if (_pollFds[0].revents & POLLIN)
			this->acceptClient();
		processUserEvents();
	}
}

void Server::acceptClient()
{
	struct sockaddr_in clientAddr;
	socklen_t clientLen = sizeof(clientAddr);
	int clientSocket = accept(_socket, (struct sockaddr*)&clientAddr, &clientLen);
	if (clientSocket == -1)
	{
		std::cerr << "Accept Error: "  << strerror(errno) << "bu error" << std::endl;
		return ;
	}

	pollfd clientPollFd;
	clientPollFd.fd = clientSocket;
	clientPollFd.events = POLLIN;
	clientPollFd.revents = 0;
	_pollFds.push_back(clientPollFd);
	_clients.insert(std::make_pair(clientSocket, Client(clientSocket)));
	std::cout << clientSocket << " Login" << std::endl;
	_clients[clientSocket].MsgToClient("Conneted the server!");
}


int Server::receiveData(Client &client)
{
	char	buffer[1024];

	std::memset(buffer, 0, sizeof(buffer));
	int bytesReceived = recv(client.getClientFd(), buffer, sizeof(buffer) - 1, 0);
	std::cout << bytesReceived << std::endl;
	if (bytesReceived <= 0) {
		if (bytesReceived == 0)
		{
			std::cout << "Client " << client.getClientFd() << " disconnected." << std::endl;
			RemoveChannels(_channels, client);
		}
		else
			std::cerr << "Error receiving data from client " << client.getClientFd() << std::endl;
		std::cout << strerror(errno) << std::endl;
	}
	buffer[bytesReceived] = '\0';
	client.appendBuffer(buffer);
	return bytesReceived;
}

void	Server::processUserEvents()
{
	for (size_t i = 1; i < _pollFds.size(); ++i)
	{
		if (_pollFds[i].revents & POLLIN)
		{
			int bytesReceived = receiveData(_clients[_pollFds[i].fd]);
			if (bytesReceived <= 0)
			{
				Quit(_channels ,_clients, _pollFds[i].fd , _pollFds);
				continue ;
			}
			processMessage(_clients[_pollFds[i].fd]);
		}
	}
}

void	Server::login(Client &client, std::vector<std::string>	&str)
{
	if (str[0] == "CAP")
		;
	else if (str[0] == "NICK")
        Nick(_clients, client, str);
	else if (str[0] == "USER")
        user(client, str);
	else if (str[0] == "QUIT")
	{
		Quit(_channels, _clients, client.getClientFd(), _pollFds);
		return ;
	}
	else if (str[0] == "PASS")
		pass(*this , client, str);
	else
		client.MsgToClient("ERROR: Please Register First!");

	if (!client.getUsername().empty() &&
		!client.getHostName().empty() &&
		!client.getNickname().empty() &&
		!client.getRealName().empty() &&
		client.getPass() == true)
	{
		client.MsgToClient(RPL_WELCOME(serverName, client.getNickname(), client.getUsername(), client.getHostName()));
		client.registerClient();
	}
}

void Server::processMessage(Client	&client)
{
	std::string	command;
	while (IsClient(_clients, client) && client.getBufferLine(command))
	{
		try
		{
			std::cout << strerror(errno) << std::endl;
			std::cout << "komut : " << command << std::endl;
			std::vector<std::string>	str = split(command, ' ');
			if (str.size() == 0)
			{
				client.MsgToClient("ERROR: Empty command");
				continue;
			}
			if (client.isRegistered() == false)
				login(client, str);
			else
				this->routeCommand(client, str);
		}
		catch(const std::string& e)
		{
			client.MsgToClient(e);
		}
	}
}

void Server::routeCommand(Client &client, std::vector<std::string> &cmd)
{
	if (cmd[0] == "USER")
		client.MsgToClient(ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0]));
    else if (cmd[0] == "NICK")
        Nick(_clients, client, cmd);
	else if (cmd[0] == "KICK")
        kick(_channels, client, cmd);
    else if (cmd[0] == "JOIN")
        Join(_channels, client, cmd);
	else if (cmd[0] == "PASS")
		pass(*this , client, cmd);
	else if (cmd[0] == "WORDL")
        Wordl::worldl(_channels ,client, cmd);
    else if (cmd[0] == "PRIVMSG")
        Privmsg(client, cmd, _channels, _clients);
	else if (cmd[0] == "QUIT")
		Quit(_channels, _clients, client.getClientFd(), _pollFds);
	else if (cmd[0] == "PART")
		Part(_channels, client, cmd);
	else if (cmd[0] == "MODE")
		Mode(_channels, client, cmd);
	else if (cmd[0] == "TOPIC")
		Topic(_channels, client, cmd);
	else if (cmd[0] == "INVITE")
		Invite(_channels, _clients ,client, cmd);
	else if (cmd[0] == "LIST")
		this->list(client);
	else if (cmd[0] == "WHO")
		who(_channels, client, cmd);
	else
		client.MsgToClient("ERROR: Unknow Command!");
}

std::string	Server::getPassword() const {return (this->_password); }


void Server::list(Client &client)
{
	client.MsgToClient(RPL_LISTSTART(client.getNickname()));
	std::map<std::string, Channel>::iterator channel;
	for (channel = _channels.begin(); channel != _channels.end(); ++channel)
	{
		std::string mes = RPL_LIST(client.getNickname(), channel->second.getName(), channel->second.getSizeClient(), channel->second.getTopic());
		client.MsgToClient(mes);
	}
	client.MsgToClient(RPL_LISTEND(client.getNickname()));
}
