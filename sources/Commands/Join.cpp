#include "Tools.hpp"
#include "Channel.hpp"
#include "error.hpp"

#include <iostream>
#include <sys/socket.h> // send
#include <map>
#include <cstring>

void Join(std::map<std::string, Channel> &channels, Client &client, std::vector<std::string> &cmd)
{
    // :user!~user@host JOIN :#mychannel  -- rpl'dir
	// if (channelName[0] != '#') {} // Eklenebilir
	if (cmd.size() > 3 || cmd.size() == 1)
	{
		client.MsgToClient(ERR_NEEDMOREPARAMS(cmd[0]));
		return ;
	}
	if (channels.find(cmd[1]) != channels.end())
	{
		std::cout << "Channel " << cmd[1] << " already exists." << std::endl;
    	std::string joinMessage = RPL_JOIN(client.getPrefixName(), cmd[1]) + "\r\n";
		channels[cmd[1]].ClientAdd(client);
		channels[cmd[1]].Brodcast(joinMessage);

		std::string nameReplyMessage = RPL_NAMREPLY(client.getPrefixName(), cmd[1], channels[cmd[1]].getUsersNames());
        std::string endOfNamesMessage = RPL_ENDOFNAMES(client.getPrefixName(), cmd[1]);
        channels[cmd[1]].Brodcast(nameReplyMessage);
        channels[cmd[1]].Brodcast(endOfNamesMessage);
	}
	else
	{
    	std::string joinMessage = RPL_JOIN(client.getPrefixName(), cmd[1]) + "\r\n";
		Channel  channel(cmd[1]);
		std::cout << "Channel Nick Name" << client.getNickname() << std::endl;

		channel.ClientAdd(client);
		channel.OperatorAdd(client);
		channels[cmd[1]] = channel;
		channels[cmd[1]].Brodcast(joinMessage);
		client.MsgToClient(RPL_NAMREPLY(client.getPrefixName(), cmd[1], channels[cmd[1]].getUsersNames()));
		client.MsgToClient(RPL_ENDOFNAMES(client.getPrefixName(), cmd[1]));
		client.MsgToClient(RPL_MODE(client.getPrefixName(), cmd[1], "+o ", client.getNickname()));

		/*int err = yolla(client.getClientFd(), message);
		if (err < 0)
			std::cerr  << std::strerror(err) << std::endl;
		else
			std::cout << "Channel Created" << std::endl;*/
	}
}
