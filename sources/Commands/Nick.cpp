#include "Client.hpp"
#include "Tools.hpp"
#include <iostream>
#include <cstring>	 	//  strerror
#include <sys/socket.h> // send
#include "Channel.hpp" // RPL and ERR
// :!bdemirbu@0 NICK bdemirbu_
void Nick(std::map<int, Client>& clients, Client &client, std::vector<std::string> cmd)
{
	if (cmd.size() != 2)
		throw ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0]);
	else if (IsClient(clients, cmd[1]) == true)
		throw ERR_NICKNAMEINUSE(cmd[1]);
	else if (client.isRegistered() == true)
	{
		std::string a = RPL_NICK(client.getPrefixName(), cmd[1]);
		std::cout << a << std::endl;
		client.MsgToClient(a);
	}
	client.setNickname(cmd[1]);
}
