#include "Client.hpp"
#include "Tools.hpp"
#include <iostream>
#include <cstring>	 	//  strerror
#include <sys/socket.h> // send
#include "Channel.hpp" // RPL and ERR

void Nick(std::map<int, Client>& clients, Client &client, std::vector<std::string> cmd)
{
	if (cmd.size() == 1)
	{
		std::cout << ":localhost 431 * " << client.getClientFd() << " nickname " << client.getNickname();
	}
	else if (IsClient(clients, cmd[1]))
	{
		client.MsgToClient(ERR_NICKNAMEINUSE(cmd[1]));
		client.setNickname(cmd[1]);
		client.setNickStatus(false);
	}
	else if (client.getNickname().empty())
	{
		client.setNickname(cmd[1]);
		return ;
	}
	else
	{
		client.setNickStatus(true);
		std::string a =  RPL_NICK(client.getPrefixName(), cmd[1]);
		client.MsgToClient(a);
		client.setNickname(cmd[1]);
	}


}
