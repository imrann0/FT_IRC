#include "Client.hpp"
#include <iostream>
#include <sys/socket.h> // send

// Default Constructor
Client::Client() :  _clientFd(-1) , _registered(false) {}

Client::Client(int fd) : _clientFd(fd) , _registered(false) {}

// Parameterized Constructor
Client::Client(const std::string& nickname, const std::string& username, int fd)
    : _clientFd(fd), _nickname(nickname), _username(username), _pass(false),  _registered(false) {}

// Getters
const std::string& Client::getNickname() const { return _nickname; }

const std::string& Client::getUsername() const { return _username; }

const std::string& Client::getRealName() const { return _realname; }

const std::string& Client::getHostName() const { return _hostname; }

const std::string& Client::getBuffer() const { return (this->_buffer); }

bool Client::isRegistered() const { return _registered; }

int Client::getClientFd() const { return _clientFd; }

bool    Client::getPass() const { return this->_pass; }

// Setters

void Client::setBuffer(const std::string& buffer) { _buffer = buffer; }

void Client::setNickname(const std::string& nickname) { _nickname = nickname; }

void Client::setUsername(const std::string& username) { _username = username; }

void Client::setRealName(const std::string& realname) { _realname = realname; }

void Client::setHostName(const std::string& hostname) { _hostname = hostname; }

void Client::registerClient() { _registered = true; }

void Client::setClientFd(int fd){ _clientFd = fd; }

void Client::setPass(bool pass) { this->_pass = pass; }

std::string	Client::getPrefixName()
{
    std::string prefix;
	std::string usname;

	if (_username.empty())
	 	usname = "";
	else
		usname = "!" + _username;

	std::string hostname;
	if (_hostname.empty())
		hostname = "";
	else
		hostname = "@" + _hostname;

	prefix = _nickname + usname + hostname;
	return prefix;
}
void Client::appendBuffer(const std::string& appendBuffer) { this->_buffer.append(appendBuffer); }

bool Client::operator==(const Client& other) const { return _clientFd == other._clientFd;}

bool Client::getBufferLine(std::string &str)
{
    unsigned long   find;

    find = this->_buffer.find("\r\n");
    if (find == std::string::npos)/* || str.compare(str.length() - 2, 2, "\r\n") */
        return (false);
    str = this->_buffer.substr(0, find);
    this->_buffer.erase(0, find + 2);
    return (true);
}

void	Client::MsgToClient(const std::string &message)
{
	std::string buf = message + "\r\n";
	if (send(_clientFd, buf.c_str(), buf.length(), 0) < 0)
		throw std::runtime_error("Send Error");
}
