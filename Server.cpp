#include "Server.hpp"
#include <sys/socket.h> // socket, bind
#include <fcntl.h>		// fcntl, F_SETFL, O_NONBLOCK
#include <cstring>	 	// memset
#include <unistd.h>		// close
#include <stdexcept>	// runtime_error
#include <poll.h>		//poll

Server::Server(int port, std::string password) : _port(port), _password(password)
{
	std::cout << "Server Constructor Called" << std::endl;

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
		int ret = poll(_pollFds.data(), _pollFds.size(), -1);
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
					handleClientEvents();
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
		return;
	}

	pollfd clientPollFd;
	clientPollFd.fd = clientSocket;
	clientPollFd.events = POLLIN;
	_pollFds.push_back(clientPollFd);
	_clients[clientSocket] = Client();

	std::cout << "New client connected: " << clientSocket << std::endl;
}

void Server::handleClientEvents()
{
	while (true)
	{
		int ret = poll(&_pollFds[0], _pollFds.size(), -1);
		if (ret == -1)
		{
			std::cerr << "Poll error!" << std::endl;
			close(_socket);
			throw std::runtime_error("poll Error");
		}

		// Loop through all users to check for events
		for (size_t i = 0; i < _pollFds.size(); ++i)
		{
			if (_pollFds[i].revents & POLLIN)
			{
				if (_pollFds[i].fd == _socket)
				{
					// Accept new client connection
					struct sockaddr_in client_addr;
					socklen_t client_size = sizeof(client_addr);
					int client_socket = accept(_socket, (struct sockaddr*)&client_addr, &client_size);
					if (client_socket < 0)
					{
						std::cerr << "Error accepting new client!" << std::endl;
						continue;
					}
					std::cout << "New connection from " << client_socket << std::endl;

					// Set up client for poll
					struct pollfd fds;
					fds.fd = client_socket;
					fds.events = POLLIN;
					_pollFds.push_back(fds);

					// Add new client to the _clients map
					Client newClient;
					_clients[client_socket] = newClient;

					// Send welcome message or other initial messages
					std::string welcomeMessage = "Welcome to the server!";
					send(client_socket, welcomeMessage.c_str(), welcomeMessage.size(), 0);
				} else
				{
					// Handle data from an existing client
					char buffer[1024];
					std::memset(buffer, 0, sizeof(buffer));

					int bytes_received = recv(_pollFds[i].fd, buffer, sizeof(buffer) - 1, 0);
					if (bytes_received <= 0)
					{
						// Client disconnected or error occurred
						if (bytes_received == 0)
							std::cout << "Client " << _pollFds[i].fd << " disconnected." << std::endl;
						else
							std::cerr << "Error receiving data from client " << _pollFds[i].fd << std::endl;

						// Remove the client from the map and close the socket
						close(_pollFds[i].fd);
						_clients.erase(_pollFds[i].fd);
						_pollFds.erase(_pollFds.begin() + i);
						continue;
					}

					// Process client data
					buffer[bytes_received] = '\0'; // Ensure null-termination
					std::cout << "Received data from client " << _pollFds[i].fd << ": " << buffer << std::endl;

					// Example of processing client input (e.g., parsing NICK or USER commands)
					if (_clients[_pollFds[i].fd].isRegistered())
					{
						// Send the response based on the client's request
						std::string response = "Message received!";
						send(_pollFds[i].fd, response.c_str(), response.size(), 0);
					} else
					{
						// Handle initial registration (e.g., NICK, USER)
						if (strncmp(buffer, "NICK", 4) == 0)
						{
							// Parse and set nickname for the client
							std::string nickname = std::string(buffer + 5);
							_clients[_pollFds[i].fd].setNickname(nickname);
							std::cout << "Client " << _pollFds[i].fd << " set nickname: " << nickname << std::endl;
						} else if (strncmp(buffer, "USER", 4) == 0)
						{
							// Parse and set username for the client
							std::string username = std::string(buffer + 5);
							_clients[_pollFds[i].fd].setUsername(username);
							std::cout << "Client " << _pollFds[i].fd << " set username: " << username << std::endl;
						}
					}
				}
			}
		}
	}
}

