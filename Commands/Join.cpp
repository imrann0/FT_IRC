#include "../Include/Command.hpp"
#include <sys/socket.h> // send
#include <map>
#include <cstring>

void Join(std::map<std::string, Channel> *channels, Client *client, std::string channelName)
{
    // :user!~user@host JOIN :#mychannel  -- rpl'dir
	channelName.erase(channelName.find_last_not_of("\r\n") + 1);
	// if (channelName[0] != '#') {} // Eklenebilir
	if  (channels->find(channelName) != channels->end())
	{
		std::cout << "Channel " << channelName << " already exists." << std::endl;
		std::string message = ":" + client->getNickname() + "!~" + client->getUsername() + "@" + client->getHostName() + " JOIN :" + channelName  + "\r\n";
		(*channels)[channelName].ClientAdd(client->getClientFd());
		yolla(client->getClientFd(), message);
	}
	else
	{
		std::string message = ":" + client->getNickname() + "!~" + client->getUsername() + "@" + client->getHostName() + " JOIN :" + channelName  + "\r\n";
		Channel  channel(channelName);
		std::cout << "Channel Nick Name" << client->getNickname() << std::endl;
		channel.ClientAdd(client->getClientFd());
		channel.OperatorAdd(client->getClientFd());
		channels->insert(std::make_pair(channelName, channel));
		int err = yolla(client->getClientFd(), message);
		if (err < 0)
			std::cerr  << std::strerror(err) << std::endl;
		else
			std::cout << "Channel Created" << std::endl;
	}
}
