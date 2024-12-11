#include "Channel.hpp"
#include "Client.hpp"
#include "Tools.hpp"
#include <iostream>

void    Invite(std::map<std::string, Channel> &channels, std::map<int, Client> &clients, Client &client, std::vector<std::string> cmd)
{
	if (cmd.size() != 3)
		throw ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0]);
	else if (cmd.size() == 3)
	{
		if (channels.find(cmd[2]) == channels.end())
			throw ERR_NOSUCHCHANNEL(client.getPrefixName(), cmd[2]);
		else if (IsClient(clients, cmd[1]) == false)
			throw (ERR_NOSUCHNICK(client.getNickname(), cmd[1]));
		else if (channels[cmd[2]].IsClient(client) == false)
			throw ERR_NOTONCHANNEL(client.getNickname(), cmd[2]);
		else if (channels[cmd[2]].IsOperator(client) == false)
			throw ERR_CHANOPRIVSNEEDED(client.getNickname(), cmd[2]);
		else
		{
				std::string in = RPL_INVITING(client.getNickname(), cmd[1], cmd[2]);
				channels[cmd[2]].setInvite(cmd[1]);
				client.MsgToClient(in);
		}
	}
}