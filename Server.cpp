#include "Server.hpp"
#include "Channel.hpp"

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
	//std::string ServerInfo =  " Giriş Yapti";
	//send(_pollFds.begin()->fd, ServerInfo.c_str(), ServerInfo.size(), 0);
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
					continue;
				}
				buffer[bytesReceived] = '\0';
				processMessage(_pollFds[i].fd, buffer, _clients);
			}

		}
	}
}



void Server::processMessage(int clientFd, const char* buffer, std::map<int, Client>& clients)
{
	std::cout << buffer << std::endl;
	if (clients[clientFd].isRegistered() == false)
	{
		std::string str = buffer;
		int p = str.find("NICK");
		if (p == -1)
			return ;
		str = str.substr(p, str.length() - p);
		if (str.find("NICK") != std::string::npos && str.find("USER") != std::string::npos)
		{
			str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
			size_t pos = str.find('\n');
			std::string nickname = str.substr(5, pos - 5);
			str = str.substr(pos + 1);


			str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
			std::string userInfo = str.substr(5); // "USER" komutundan sonraki kısmı alıyoruz
			size_t firstSpace = userInfo.find(" "); // ilk boşluğu bul
			size_t secondSpace = userInfo.find(" ", firstSpace + 1); // ikinci boşluğu bul
			size_t thirdSpace = userInfo.find(" ", secondSpace + 1); // üçüncü boşluğu bul

			// Parçaları ayıralım
			std::string username = userInfo.substr(0, firstSpace); // username
			std::string hostname = userInfo.substr(firstSpace + 1, secondSpace - firstSpace - 1); // hostname
			std::string servername = userInfo.substr(secondSpace + 1, thirdSpace - secondSpace - 1); // servername
			std::string realname = userInfo.substr(thirdSpace + 2); // realname kısmını almak için ':'

			// Gerçek adı temizlemek için trim işlemi yapılabilir
			if (realname[0] == ':') {
				realname = realname.substr(1); // ':' karakterini çıkar
			}
			clients[clientFd].setNickname(nickname);
			clients[clientFd].setUsername(username);
			clients[clientFd].setRealName(realname);
			clients[clientFd].setHostName(hostname);
			clients[clientFd].registerClient();
			std::cout << clientFd << " Başarılı Bir Şekilde Kayıt Gerçekleşti" << std::endl;
			/*std::cout << "nickname: " << nickname << std::endl;
			std::cout << "username: " << username << std::endl;
			std::cout << "hostname: " << hostname << std::endl;
			std::cout << "servername: " << servername << std::endl;
			std::cout << "realname: " << realname << std::endl;*/
		}
	}
	else if (clients[clientFd].isRegistered() == false) //hatalı kullanıum düzeltilcek kayıt olmayan kullanıcı nick komutunu kullanamıyor
	{
		std::string response = "Please Register First!";
		send(clientFd, response.c_str(), response.size(), 0);
	}
	else
	{
		if (strncmp(buffer, "NICK", 4) == 0)
		{
			std::string nickname(buffer + 5);
			nickname.erase(nickname.find_last_not_of("\r\n") + 1);
			if (nickname.length() == 0)
			{
				std::cout << ":localhost 431 * " << clientFd << " nickname " << clients[clientFd].getNickname();
			}
			//std::string nickname = str.substr(0, str.length());
			std::string a =  ":" + clients[clientFd].getNickname() + "!" + clients[clientFd].getUsername() + "@localhost NICK " + nickname + "\r\n";
			int err = send(clientFd, a.c_str(), a.length(), 0);
			if (err < 0)
				std::cerr  << std::strerror(err) << std::endl;
			else
				std::cout << "NICK UPDATE SUCCSES" << std::endl;
/*
			std::cout << clientFd <<std::endl;
			std::cout << clients[clientFd].getNickname()  <<std::endl;
			std::cout << clients[clientFd].getUsername() <<std::endl;
			std::cout << nickname <<std::endl; */

			clients[clientFd].setNickname(nickname);
			std::cout << "Client " << clientFd << " set nickname: " << nickname << std::endl;
		}
		else if (strncmp(buffer, "USER", 4) == 0)
		{
			//eklencek
			std::string a = buffer;
		}
		else if (strncmp(buffer, "GETNICK", 7) == 0)
		{
			//Test Komutu
			std::string message = clients[clientFd].getNickname() + "\r\n";
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
				std::string message = ":" + clients[clientFd].getUsername() + "!~" + clients[clientFd].getUsername() + "@" + clients[clientFd].getHostName() + " JOIN :" + channelName  + "\r\n";
				_channels[channelName].ClientAdd(clients[clientFd]);
				send(clientFd, message.c_str(), message.length(), 0);
			}
			else
			{
				std::string message = ":" + clients[clientFd].getUsername() + "!~" + clients[clientFd].getUsername() + "@" + clients[clientFd].getHostName() + " JOIN :" + channelName  + "\r\n";
				Channel  channel(channelName);
				channel.ClientAdd(clients[clientFd]);
				channel.OperatorAdd(clients[clientFd]);
				_channels.insert(std::make_pair(channelName, channel));
				send(clientFd, message.c_str(), message.length(), 0);
			}
			std::cout << "1" << std::endl;
		}
		else if (strncmp(buffer, "PRIVMSG", 7 ) == 0)
		{
			// :Alice!alice@irc.example.com PRIVMSG #general :Herkese merhaba!
			// :<sender>!<user>@<host> PRIVMSG <channel> :<message>
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
			std::string content = rawMessage.substr(secondSpace + 2); // Mesaj içeriği
			//std::cout << target << " " << content << std::endl;
			//std::string m = ":" + clients[clientFd].getNickname() + "!" + clients[clientFd].getUsername() + "@" + clients[clientFd].getHostName() + " "+ message;
			std::vector<Client> users = _channels[target].getClients();
			it user = users.begin() + 1;
			for (; user != users.end(); user++)
			{
				int fda = user->getClientFd();
				std::string m = ":" + clients[fda].getNickname() + "!" + clients[fda].getUsername() + "@" + clients[fda].getHostName() + " "+ rawMessage + "\r\n";
				send(fda, m.c_str(), m.length(), 0);
			}
			//send(clientFd, m.c_str(), m.length(), 0);

		}
	}
}
