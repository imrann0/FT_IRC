#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <map>
#include <sstream>

#define PORT 6667

// Global değişkenler
std::map<int, std::string> clients; // Soket ve takma adlar
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;

// Mesaj gönderme fonksiyonu
void send_message(int sender_socket, const std::string& message) {
	pthread_mutex_lock(&client_mutex);
	for (std::map<int, std::string>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if (it->first != sender_socket) {
			send(it->first, message.c_str(), message.size(), 0);
		}
	}
	pthread_mutex_unlock(&client_mutex);
}

// İstemci işleme fonksiyonu
void* handle_client(void* arg) {
	int client_socket = *(int*)arg;
	delete (int*)arg; // Dinamik olarak ayrılan bellek temizliği

	char buffer[1024];
	std::string nickname;

	// Takma ad belirleme
	while (true) {
		memset(buffer, 0, sizeof(buffer));
		int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
		if (bytes_received <= 0) break;

		std::string input(buffer);
		if (input.substr(0, 5) == "NICK ") {
			nickname = input.substr(5);
			nickname.erase(nickname.find_last_not_of("\n\r") + 1); // Yeni satırları sil
			pthread_mutex_lock(&client_mutex);
			clients[client_socket] = nickname;
			pthread_mutex_unlock(&client_mutex);
			send(client_socket, "Welcome to the IRC server!\n", 27, 0);
			break;
		} else {
			send(client_socket, "Set your nickname first using NICK <name>\n", 42, 0);
		}
	}

	// Mesajlaşma
	while (true) {
		memset(buffer, 0, sizeof(buffer));
		int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
		if (bytes_received <= 0) break;

		std::string input(buffer);
		if (input.substr(0, 8) == "PRIVMSG ") {
			size_t colon_pos = input.find(":");
			if (colon_pos != std::string::npos) {
				std::string target = input.substr(8, colon_pos - 9);
				std::string message = input.substr(colon_pos + 1);

				pthread_mutex_lock(&client_mutex);
				for (std::map<int, std::string>::iterator it = clients.begin(); it != clients.end(); ++it) {
					if (it->second == target) {
						std::ostringstream oss;
						oss << nickname << " --> " << target << ": " << message;
						send(it->first, oss.str().c_str(), oss.str().size(), 0);
						break;
					}
				}
				pthread_mutex_unlock(&client_mutex);
			}
		} else if (input == "QUIT\n") {
			break;
		} else {
			send(client_socket, "Invalid command.\n", 18, 0);
		}
	}

	// Bağlantıyı sonlandır
	pthread_mutex_lock(&client_mutex);
	clients.erase(client_socket);
	pthread_mutex_unlock(&client_mutex);
	close(client_socket);
	pthread_exit(NULL);
}

// Sunucu başlatma
int main() {
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		std::cerr << "Socket creation failed.\n";
		return 1;
	}

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
		std::cerr << "Binding failed.\n";
		close(server_socket);
		return 1;
	}

	if (listen(server_socket, 5) == -1) {
		std::cerr << "Listening failed.\n";
		close(server_socket);
		return 1;
	}

	std::cout << "IRC server started on port " << PORT << ".\n";

	while (true) {
		sockaddr_in client_addr;
		socklen_t client_size = sizeof(client_addr);
		int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_size);

		if (client_socket == -1) {
			std::cerr << "Failed to accept client.\n";
			continue;
		}

		std::cout << "Client connected.\n";
		int* new_socket = new int(client_socket);
		pthread_t client_thread;
		pthread_create(&client_thread, NULL, handle_client, new_socket);
		pthread_detach(client_thread);
	}

	close(server_socket);
	return 0;
}
