#include "Server.hpp"
#include "Channel.hpp"
#include "Tools.hpp"
#include "Command.hpp"

#include <sys/socket.h> // socket, bind
#include <fcntl.h>		// fcntl, F_SETFL, O_NONBLOCK
#include <cstring>	 	// memset strerror
#include <unistd.h>		// close
#include <stdexcept>	// runtime_error
#include <poll.h>		//poll
#include <string>
#include <cerrno>   // errno
#include <algorithm> // std::error


Server::Server(int port, std::string password) : _port(port), _password(password)
{
	std::cout << "Server Constructor Called" << std::endl;
	serverName  = "irc.myserver.com";
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
		throw std::runtime_error("Socket Error");
	int buffer_size = 1;
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

	if (listen(_socket, 5) == -1)
	{
		close(_socket);
		throw std::runtime_error("Listen Error");
	}

	struct pollfd fds;
	fds.fd = _socket;
	fds.events = POLLIN;
	_pollFds.push_back(fds);
}

void	Server::Debug()
{
	while (true)
	{
		int ret = poll(_pollFds.data(), _pollFds.size(), -1); // poll nedir?
		if (ret == -1)
		{
			close(_socket);
			throw std::runtime_error("poll Error");
		}
		for (size_t i = 0; i < _pollFds.size(); ++i)
		{
			if (_pollFds[i].revents & POLLIN)
			{
				if (_pollFds[i].fd == _socket)
					acceptClient();
				else
					processUserEvents();
			}
		}
	}
}

void Server::acceptClient()
{
	sockaddr_in clientAddr;
	socklen_t clientLen = sizeof(clientAddr);
	int clientSocket = accept(_socket, (struct sockaddr*)&clientAddr, &clientLen);
	if (clientSocket == -1)
	{
		std::cerr << "Accept Error" << std::endl;
		return ;
	}

	pollfd clientPollFd;
	clientPollFd.fd = clientSocket;
	clientPollFd.events = POLLIN;
	_pollFds.push_back(clientPollFd);
	_clients.insert(std::make_pair(clientSocket, Client(clientSocket)));
	std::cout << clientSocket << " Login" << std::endl;
	yolla(clientSocket, "Welcome to the server!\n");
}


int receiveData(Client &client)
{
	char	buffer[1024];

	std::memset(buffer, 0, sizeof(buffer));
	int bytesReceived = recv(client.getClientFd(), buffer, sizeof(buffer) - 1, 0);
	if (bytesReceived <= 0) {
		if (bytesReceived == 0)
			std::cout << "Client " << client.getClientFd() << " disconnected." << std::endl;
		else
			std::cerr << "Error receiving data from client " << client.getClientFd() << std::endl;
	}
	buffer[bytesReceived] = '\0';
	client.appendBuffer(buffer);
	std::cout << client.getBuffer() << std::endl;
	return bytesReceived;
}

void	Server::processUserEvents()
{
	for (size_t i = 0; i < _pollFds.size(); ++i)
	{
		if (_pollFds[i].revents & POLLIN)
		{
			if (_pollFds[i].fd == _socket)
				Server::acceptClient();
			else
			{
				int bytesReceived = receiveData(_clients[_pollFds[i].fd]);
				if (bytesReceived <= 0)
				{
					Quit(_pollFds[i].fd, _clients, _pollFds);
					continue ;
				}
				processMessage(_clients[_pollFds[i].fd]);
			}

		}
	}
}

/*
	if (str.find("NICK") != std::string::npos &&
		str.find("USER") != std::string::npos)
	{
		int p = str.find("NICK");
		str = str.substr(p, str.length() - p);
		chatRegisterClient(str, &client);
	}
	else if (str.find("NICK") != std::string::npos ||
		str.find("USER") != std::string::npos)
	{

	} */

void	login(Server &server, Client &client, std::string &str)
{
	if (str.compare(0, 10, "CAP LS 302") == 0)
		;
	else if (str.compare(0, 4, "NICK") == 0)
        Nick(client, str.substr(5));
	else if (str.compare(0, 4, "USER") == 0)
        user(client, str.substr(5));
	else if (str.compare(0, 4, "PASS") == 0)
		pass(server, client, str.substr(5));
	else
		yolla(client.getClientFd(), "ERROR: Please Register First!");
	if (!client.getUsername().empty() &&
		!client.getHostName().empty() &&
		!client.getNickname().empty() &&
		!client.getRealName().empty() &&
		client.getPass() != true)
		client.registerClient();
}

void Server::processMessage(Client	&client)
{
	std::string	str;

	while (client.getBufferLine(str))
	{
		if (client.isRegistered() == false)
			login(*this, client, str);
		else
			this->routeCommand(client.getClientFd(), str);
	}
}

void Server::routeCommand(int clientFd, const std::string& str)
{
    if (str.compare(0, 4, "NICK") == 0)
        Nick(this->_clients[clientFd], str.substr(5));
    else if (str.compare(0, 4, "JOIN") == 0)
        Join(_channels, _clients[clientFd], str.substr(5));
    else if (str.compare(0, 7, "PRIVMSG") == 0)
        Privmsg(_clients[clientFd], str, _channels, _clients);
	else if (str.compare(0, 4, "QUIT") == 0)
		Quit(clientFd, _clients, _pollFds);
	else if (str.compare(0, 4, "PART") == 0)
		Part(_channels, _clients[clientFd], str.substr(5));
}

std::string	Server::getPassword() const {return (this->_password); }
