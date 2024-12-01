#include "Client.hpp"

// Default Constructor
Client::Client() :  _clientFd(-1) , _registered(false) {}

Client::Client(int fd) : _clientFd(fd) , _registered(false) {}

// Parameterized Constructor
Client::Client(const std::string& nickname, const std::string& username, int fd)
    : _clientFd(fd), _nickname(nickname), _username(username),  _registered(false) {}

// Getters
const std::string& Client::getNickname() const { return _nickname; }

const std::string& Client::getUsername() const { return _username; }

const std::string& Client::getRealName() const { return _realname; }

const std::string& Client::getHostName() const { return _hostname; }


bool Client::isRegistered() const { return _registered; }

int Client::getClientFd() const { return _clientFd; }

// Setters
void Client::setNickname(const std::string& nickname) { _nickname = nickname; }

void Client::setUsername(const std::string& username) { _username = username; }

void Client::setRealName(const std::string& realname) { _realname = realname; }

void Client::setHostName(const std::string& hostname) { _hostname = hostname; }

void Client::registerClient() { _registered = true; }


void Client::setClientFd(int fd){ _clientFd = fd; }


std::string	Client::getPrefixName() {
    std::string prefixName = _nickname;
    if (!(_username.empty()))
        prefixName += '!' + _username;
    if (!(_hostname.empty()))
        prefixName += '@' + _hostname;
    return (prefixName);
}
