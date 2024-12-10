#include "Tools.hpp"
#include "Channel.hpp"
#include "error.hpp"

#include <iostream>
#include <sys/socket.h> // send
#include <map>
#include <cstring>

void Join(std::map<std::string, Channel> &channels, Client &client, std::vector<std::string> &cmd)
{
	if (cmd.size() > 3 || cmd.size() == 1)
		throw ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0]);
	std::string	password = cmd.size() == 3 ? cmd[2] : "" ;
	if (channels.find(cmd[1]) != channels.end())
	{
		if (channels[cmd[1]].IsFlags('k') && channels[cmd[1]].getPassword() != password)
			throw ERR_BADCHANNELKEY(client.getNickname(), channels[cmd[1]].getName());
		if (channels[cmd[1]].IsFlags('l') && channels[cmd[1]].getLimit() == false)
			throw (ERR_CHANNELISFULL(client.getNickname(), cmd[1]));
		if (channels[cmd[1]].IsFlags('i') == true && channels[cmd[1]].IsInvites(client.getNickname()) == false)
			throw (ERR_INVITEONLYCHAN(client.getNickname(), cmd[1]));

		std::string joinMessage = RPL_JOIN(client.getPrefixName(), cmd[1]);
		channels[cmd[1]].ClientAdd(client);
		channels[cmd[1]].Brodcast(joinMessage);

		std::string nameReplyMessage = RPL_NAMREPLY(client.getPrefixName(), cmd[1], channels[cmd[1]].getUsersNames());
		std::string endOfNamesMessage = RPL_ENDOFNAMES(client.getPrefixName(), cmd[1]);
		channels[cmd[1]].Brodcast(nameReplyMessage);
		channels[cmd[1]].Brodcast(endOfNamesMessage);
	}
	else
	{
    	std::string joinMessage = RPL_JOIN(client.getPrefixName(), cmd[1]);
		Channel  channel(cmd[1], password);
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
