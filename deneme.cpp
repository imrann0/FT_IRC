#include <sys/socket.h>	// socket, bind
#include <iostream>
#include <string>
#include <cstring>		// memset
#include <netinet/in.h>	// sockaddr_in
#include <unistd.h>	 // close
#include <poll.h>  //poll
#include <fcntl.h> // fcntl, F_SETFL, O_NONBLOCK
#include <vector> // vector
#include <map>
#include <utility>
#include <fstream>
#include <cstdlib>
// https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.1

#define PORT 6667
#define RPL_JOIN(nick, user, host, channel) (":" + (nick) + "!" + (user) + "@" + (host) + " JOIN " + (channel) + "\r\n")

int main()
{
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	int buffer_size = 1024 * 1024;  // 1 MB
	if (server_socket == -1)
	{
		std::cout << "socket error" << std::endl;
		return 1;
	}
	// bu ne işe yarar bak
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &buffer_size, sizeof(buffer_size)) == -1) {
		perror("setsockopt error");
	}

	if (fcntl(server_socket, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "fcntl F_SETFL hatası" << std::endl;
		return 1;
	}
	struct sockaddr_in server_addr;

	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;					// IPv4 adres ailesi
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	// Herhangi bir IP adresi
	server_addr.sin_port = htons(PORT);					// Port numarası

	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		std::cout << "Bind Error" << std::endl;
		close(server_socket);
		return 1;
	}
	std::cout << "Bind başarılı. Sunucu " << PORT << " portuna bağlandı.\n";


	if (listen(server_socket, 2) == -1)
	{
		std::cerr << "Listening Error." << std::endl;
		close(server_socket);
		return 1;
	}

	struct pollfd fds[10];  // Maksimum 10 istemci dinleyeceğiz
	int client_count = 0;
	std::vector<pollfd> userfd;
	std::map<std::string, std::vector<int> > channels; // Kanal adı ve istemci fd'leri
	fds[0].fd = server_socket;
	fds[0].events = POLLIN;
	userfd.push_back(fds[0]);
	while (true)
	{
		int ret = poll(&userfd[0], userfd.size(), -1);
		if (ret == -1)
		{
			std::cout << "poll error" << std::endl;
			return (1);
		}
		if (userfd[0].revents & POLLIN)
		{
			struct sockaddr_in client_addr;
			socklen_t client_size = sizeof(client_addr);
			int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_size);
			if(client_socket < 0)
			{
				fprintf(stderr, "İsteklerin kabulü sırasında hata oluştu.");
					return 1;
			}
			std::cout << "connection" << std::endl;
			fds[client_count + 1].fd = client_socket;
			fds[client_count + 1].events = POLLIN;
			userfd.push_back(fds[client_count + 1]);
			client_count++;

			 if (client_count == 10) {
				std::cout << "Maksimum istemci sayısına ulaşıldı." << std::endl;
			}
		}

		for (int i = 1; i <= client_count; ++i)
		{
			if (userfd[i].revents & POLLIN)
			{
				char buffer[1024];
				int bytes_received = recv(userfd[i].fd, buffer, sizeof(buffer), 0);
				std::string message = buffer;

				if (bytes_received > 0)
				{
					buffer[bytes_received] = '\0';
					if (message.find("JOIN") == 0)
					{
						std::string channel_name = message.substr(4); // "JOIN" sonrası
						channel_name.erase(0, channel_name.find_first_not_of(" \t\r\n"));
						channel_name.erase(channel_name.find_last_not_of(" \t\r\n") + 1);
						if (channels.find(channel_name) == channels.end())
						{
							// Kanal yoksa oluştur
							channels[channel_name] = std::vector<int>();
							std::cout << "Yeni kanal oluşturuldu: " << channel_name << std::endl;
						}
						// İstemciyi kanala ekle
						channels[channel_name].push_back(userfd[i].fd);
						std::string nick = "client" + std::to_string(i);  // Geçici kullanıcı adı
                        std::string user = "user" + std::to_string(i);    // Geçici kullanıcı adı
                        std::string host = "localhost";                    // Sunucu adı
                        std::string join_message = ":" + nick + "!" + user + "@" + host + " JOIN " + channel_name + "\r\n";
                        send(userfd[i].fd, join_message.c_str(), join_message.size(), 0);
						std::cout << "İstemci " << userfd[i].fd << " kanala katıldı: " << channel_name << std::endl;
					}
					std::cout << "İstemci " << userfd[i].fd << " mesaj aldı: " << buffer << std::endl;
				}
				else if (bytes_received == 0)
				{
					std::cout << "İstemci " << userfd[i].fd << " bağlantıyı kapattı." << std::endl;
					close(userfd[i].fd);  // İstemciyi kapat
					userfd[i] = userfd[client_count];  // Son istemciyi bu yere taşı
					client_count--;
				}
				else
				{
					std::cerr << "Veri alma hatası!" << std::endl;
					close(userfd[i].fd);  // Hata durumunda istemciyi kapat
					userfd[i] = userfd[client_count];  // Son istemciyi bu yere taşı
					client_count--;
				}
			}
		}
	}
	close(server_socket);
	return (0);
}
