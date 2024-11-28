#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>
#include "Client.hpp"
typedef std::vector<Client>::iterator it;

class Channel
{
	private:
		std::string _name;
		std::vector<Client> _clients;

	public:
		Channel();
		Channel(const std::string& name);
		const std::string& getName() const;
		// Kanala bir istemci ekle
		void addClient(Client newClient);
		// Kanaldan bir istemci çıkar
		void removeClient(Client rClient);
		// Kanal üyelerine mesaj gönder
		void broadcast(const std::string& message, int senderFd);
		std::string getUsers() const;
};

#endif
