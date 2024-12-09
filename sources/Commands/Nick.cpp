#include "Client.hpp"
#include "Tools.hpp"
#include <iostream>
#include <cstring>	 	//  strerror
#include <sys/socket.h> // send
#include "Channel.hpp" // RPL and ERR
void Nick(std::map<int, Client>& clients, Client &client, std::vector<std::string> cmd)
{

	if (IsClient(clients, cmd[1]))
	{
		client.MsgToClient(ERR_NICKNAMEINUSE(cmd[1]));
		return ;
	}
	std::string a =  ":" + client.getNickname() + "!" + client.getUsername() + "@localhost NICK " + cmd[1] + "\r\n";
	client.MsgToClient(a);
	client.setNickname(cmd[1]);
}
