#include "Tools.hpp"
#include "Channel.hpp"
#include <iostream>
#include <sys/socket.h> // send
#include <map>
#include <cstring>

void Join(std::map<std::string, Channel> &channels, Client &client, std::string channelName)
{
    // :user!~user@host JOIN :#mychannel  -- rpl'dir
	channelName.erase(channelName.find_last_not_of("\r\n") + 1);
	// if (channelName[0] != '#') {} // Eklenebilir
	if  (channels.find(channelName) != channels.end())
	{
		std::cout << "Channel " << channelName << " already exists." << std::endl;
    	std::string joinMessage = RPL_JOIN(client.getPrefixName(), channelName) + "\r\n";
		channels[channelName].ClientAdd(client);
		channels[channelName].Brodcast(joinMessage);

		std::string nameReplyMessage = RPL_NAMREPLY(client.getPrefixName(), channelName, channels[channelName].getUsersNames()) + "\r\n";
        std::string endOfNamesMessage = RPL_ENDOFNAMES(client.getPrefixName(), channelName) + "\r\n";
        channels[channelName].Brodcast(nameReplyMessage);
        channels[channelName].Brodcast(endOfNamesMessage);
	}
	else
	{
    	std::string joinMessage = RPL_JOIN(client.getPrefixName(), channelName) + "\r\n";
		Channel  channel(channelName);
		std::cout << "Channel Nick Name" << client.getNickname() << std::endl;

		channel.ClientAdd(client);
		channel.OperatorAdd(client);
		channels[channelName] = channel;
		channels[channelName].Brodcast(joinMessage);
		client.MsgToClient(RPL_NAMREPLY(client.getPrefixName(), channelName, channels[channelName].getUsersNames()));
		client.MsgToClient(RPL_ENDOFNAMES(client.getPrefixName(), channelName));
		client.MsgToClient(RPL_MODE(client.getPrefixName(), channelName, "+o ", client.getNickname()));

		/*int err = yolla(client.getClientFd(), message);
		if (err < 0)
			std::cerr  << std::strerror(err) << std::endl;
		else
			std::cout << "Channel Created" << std::endl;*/
	}
}
