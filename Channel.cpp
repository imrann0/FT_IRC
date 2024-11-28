#include "Channel.hpp"
#include <sys/socket.h>

Channel::Channel(): _name("NULL") {}
Channel::Channel(const std::string& name): _name(name) {}

const std::string& Channel::getName() const {return _name;}


void Channel::addClient(Client newClient) {_clients.push_back(newClient);}

void Channel::removeClient(Client rClient)
{
	it client = std::find(_clients.begin(), _clients.end(), rClient);
	if (client != _clients.end())
		_clients.erase(client);
}

void Channel::broadcast(const std::string& message, int senderFd)
{
    for (it i = _clients.begin(); i != _clients.end(); ++i) {
        if (i->getClientFd() != senderFd) {  // Gönderen istemci hariç
            send(i->getClientFd(), message.c_str(), message.length(), 0);
        }
    }
}


std::string Channel::getUsers() const {
    std::string users;
    for (size_t i = 0; i < _clients.size(); ++i) {
        users += _clients[i].getNickname();  // Kullanıcı takma adı
        if (i != _clients.size() - 1) {
            users += " ";  // Boşluk ekle
        }
    }
    return users;
}

