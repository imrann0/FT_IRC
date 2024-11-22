#include "Client.hpp"

// Default Constructor
Client::Client() : _registered(false) {}

// Parameterized Constructor
Client::Client(const std::string& nickname, const std::string& username)
    : _nickname(nickname), _username(username), _registered(false) {}

// Getters
const std::string& Client::getNickname() const {
    return _nickname;
}

const std::string& Client::getUsername() const {
    return _username;
}

const std::string& Client::getRealName() const {
    return _realname;
}

bool Client::isRegistered() const {
    return _registered;
}

// Setters
void Client::setNickname(const std::string& nickname) {
    _nickname = nickname;
}

void Client::setUsername(const std::string& username) {
    _username = username;
}

void Client::setRealName(const std::string& realname) {
    _realname = realname;
}

void Client::registerClient() {
    _registered = true;
}
