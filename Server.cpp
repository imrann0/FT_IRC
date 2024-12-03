#include "Server.hpp"
#include "Channel.hpp"
#include "Tools.hpp"
#include "Include/Command.hpp"

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
		return ;
	}

	pollfd clientPollFd;
	clientPollFd.fd = clientSocket;
	clientPollFd.events = POLLIN;
	_pollFds.push_back(clientPollFd);
	_clients.insert(std::make_pair(clientSocket, Client(clientSocket)));

	std::string welcomeMessage = "Welcome to the server!\n";
	std::cout << clientSocket << " Login" << std::endl;
	send(clientSocket, welcomeMessage.c_str(), welcomeMessage.size(), 0);
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
		processUserEvents();
	}
}
int receiveData(int clientFd, char* buffer, size_t bufferSize)
{
	std::memset(buffer, 0, bufferSize);
	int bytesReceived = recv(clientFd, buffer, bufferSize - 1, 0);

	if (bytesReceived <= 0) {
		if (bytesReceived == 0)
			std::cout << "Client " << clientFd << " disconnected." << std::endl;
		else
			std::cerr << "Error receiving data from client " << clientFd << std::endl;
	}
	return bytesReceived;
}

void closeConnection(int clientFd, std::map<int, Client>& clients, std::vector<pollfd>& pollFds, size_t index)
{
	close(clientFd);
	clients.erase(clientFd);
	pollFds.erase(pollFds.begin() + index);
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
				char buffer[1024];
				int bytesReceived = receiveData(_pollFds[i].fd, buffer, sizeof(buffer));
				if (bytesReceived <= 0)
				{
					closeConnection(_pollFds[i].fd, _clients, _pollFds, i);
					continue ;
				}
				buffer[bytesReceived] = '\0';
				processMessage(_pollFds[i].fd, buffer);
			}

		}
	}
}

const Client Server::getClientNameFd(std::string& target)
{
	std::map<int, Client>::iterator user = _clients.begin();
	for (; user != _clients.end(); user++)
	{
		if (user->second.getNickname() == target)
			return (user->second);
	}
	return (user->second); // bu olmayan bir şeyde ne döner diye testinin yapılması lazım // kesin hata
}


void Server::processMessage(int clientFd, const char* buffer)
{
	std::cout << buffer << std::endl;
	if (_clients[clientFd].isRegistered() == false)
	{
		std::string str = buffer;
		int p = str.find("NICK");
		if (p == -1)
			return ;
		str = str.substr(p, str.length() - p);
		if (str.find("NICK") != std::string::npos && str.find("USER") != std::string::npos)
		{
			chatRegisterClient(str, &_clients[clientFd]);
		}
	}
	else if (_clients[clientFd].isRegistered() == false) //hatalı kullanıum düzeltilcek kayıt olmayan kullanıcı nick komutunu kullanamıyor
	{
		std::string response = "Please Register First!";
		send(clientFd, response.c_str(), response.size(), 0);
	}
	else
	{
		if (strncmp(buffer, "NICK", 4) == 0)
		{
			Nick(&_clients[clientFd], buffer + 5);
		}
		else if (strncmp(buffer, "USER", 4) == 0)
		{
			//eklencek
			std::string a = buffer;
		}
		else if (strncmp(buffer, "GETNICK", 7) == 0)
		{
			//Test Komutu
			std::string message = _clients[clientFd].getNickname() + "\r\n";
			if (send(clientFd, message.c_str(), message.length(), 0) == -1)
				std::cerr << "Error sending message to client!" << std::endl;
		}
		else if (strncmp(buffer, "JOIN", 4) == 0)
		{
			// :user!~user@host JOIN :#mychannel  -- rpl'dir
			std::cout << "3" << std::endl;
			std::string channelName(buffer + 5);
			channelName.erase(channelName.find_last_not_of("\r\n") + 1);
			// if (channelName[0] != '#') {} // Eklenebilir
			if  (_channels.find(channelName) != _channels.end())
			{
				std::cout << "Channel " << channelName << " already exists." << std::endl;
				std::string message = ":" + _clients[clientFd].getNickname() + "!~" + _clients[clientFd].getUsername() + "@" + _clients[clientFd].getHostName() + " JOIN :" + channelName  + "\r\n";
				_channels[channelName].ClientAdd(_clients[clientFd]);
				send(clientFd, message.c_str(), message.length(), 0);
			}
			else
			{
				std::string message = ":" + _clients[clientFd].getNickname() + "!~" + _clients[clientFd].getUsername() + "@" + _clients[clientFd].getHostName() + " JOIN :" + channelName  + "\r\n";
				Channel  channel(channelName);
				std::cout << "Channel Nick Name" << _clients[clientFd].getNickname() << std::endl;
				channel.ClientAdd(_clients[clientFd]);
				channel.OperatorAdd(_clients[clientFd]);
				_channels.insert(std::make_pair(channelName, channel));
				int err = send(clientFd, message.c_str(), message.length(), 0);
				if (err < 0)
					std::cerr  << std::strerror(err) << std::endl;
				else
					std::cout << "NICK UPDATE SUCCSES" << std::endl;
			}
			std::cout << "1" << std::endl;
		}
		else if (strncmp(buffer, "PRIVMSG", 7 ) == 0)
		{
			// :Alice!alice@irc.example.com PRIVMSG #general :Herkese merhaba!
			// :<sender>!<user>@<host> PRIVMSG <channel> :<message>
			// :<sender_nickname>!<sender_username>@<sender_host> PRIVMSG <recipient_nickname> :<message> ->özel mesaj
			std::string rawMessage(buffer);
			rawMessage.erase(rawMessage.find_last_not_of("\r\n") + 1);

			// Mesajı parçalara ayır (alıcı ve içerik)
			size_t firstSpace = rawMessage.find(" ");
			size_t secondSpace = rawMessage.find(" ", firstSpace + 1);

			if (firstSpace == std::string::npos || secondSpace == std::string::npos) {
				// Geçersiz format
				std::string error = "ERROR :Invalid PRIVMSG format\r\n";
				send(clientFd, error.c_str(), error.length(), 0);
				return;
			}

			std::string target = rawMessage.substr(firstSpace + 1, secondSpace - firstSpace - 1); // Alıcı (#channel veya kullanıcı)
			std::string content = rawMessage.substr(secondSpace + 1); // Mesaj içeriği
			if (target[0] == '#')
			{
				std::vector<Client> users = _channels[target].getClients();
				it user = users.begin();
				for (; user != users.end(); user++)
				{
					if (user->getClientFd() == clientFd)
						continue ;
					int fda = user->getClientFd();
					std::string m = ":" + _clients[clientFd].getNickname() + "!" + _clients[fda].getUsername() + "@" + _clients[fda].getHostName() + " "+ rawMessage + "\r\n";
					send(fda, m.c_str(), m.length(), 0);
				}
			}
			else
			{
				Client targetCLient = this->getClientNameFd(target);
				std::cout << "Target NickName: " <<  targetCLient.getNickname() << std::endl;
				std::cout << "content: " << content << std::endl;
				std::string mes = ":" + _clients[clientFd].getNickname() + "!~" +  _clients[clientFd].getUsername() + "@" + _clients[clientFd].getHostName() + " PRIVMSG " + targetCLient.getNickname() + " :" + content + "\r\n";
				send(targetCLient.getClientFd(), mes.c_str(), mes.length(), 0);
				//send(clientFd, mes.c_str(), mes.length(), 0);
			}

		}
	}
}
