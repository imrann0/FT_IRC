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
		client.MsgToClient(ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0]));
	else if (channels.find(cmd[1]) != channels.end())
	{
		if (channels[cmd[1]].IsFlags('l'))
		{
			if (channels[cmd[1]].getLimit() == false)
				client.MsgToClient(ERR_CHANNELISFULL(client.getNickname(), cmd[1]));
		}
		else if (channels[cmd[1]].IsFlags('i') == true && channels[cmd[1]].IsInvites(client.getNickname()) == false)
			client.MsgToClient(ERR_INVITEONLYCHAN(client.getNickname(), cmd[1]));
		else
		{
			std::string joinMessage = RPL_JOIN(client.getPrefixName(), cmd[1]);
			channels[cmd[1]].ClientAdd(client);
			channels[cmd[1]].Brodcast(joinMessage);

			std::string nameReplyMessage = RPL_NAMREPLY(client.getPrefixName(), cmd[1], channels[cmd[1]].getUsersNames());
			std::string endOfNamesMessage = RPL_ENDOFNAMES(client.getPrefixName(), cmd[1]);
			channels[cmd[1]].Brodcast(nameReplyMessage);
			channels[cmd[1]].Brodcast(endOfNamesMessage);
		}
	}
	else
	{
    	std::string joinMessage = RPL_JOIN(client.getPrefixName(), cmd[1]);
		Channel  channel(cmd[1]);
		std::cout << "Channel Nick Name" << client.getNickname() << std::endl;

		channel.ClientAdd(client);
		channel.OperatorAdd(client);
		channels[cmd[1]] = channel;
		channels[cmd[1]].Brodcast(joinMessage);
		client.MsgToClient(RPL_NAMREPLY(client.getPrefixName(), cmd[1], channels[cmd[1]].getUsersNames()));
		client.MsgToClient(RPL_ENDOFNAMES(client.getPrefixName(), cmd[1]));
		client.MsgToClient(RPL_MODE(client.getPrefixName(), cmd[1], "+o ", client.getNickname()));
	}
}
