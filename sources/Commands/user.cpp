#include "Client.hpp"
#include "Channel.hpp" // Err hata kodu için eklendi
#include "Tools.hpp"
#include "error.hpp"

#include <iostream>

/*
	Command: USER
	Parameters: <username> <hostname> <servername> <realname>
	Numeric Replies:
           ERR_NEEDMOREPARAMS :
		   ERR_ALREADYREGISTRED : "<command> :Not enough parameters"
 */
void	user(Client &client, std::vector<std::string> cmd)
{
	if (cmd.size() != 5)
	{
		client.MsgToClient(ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0]));
		return;
	}
	if (cmd[4][0] == ':') {
		cmd[4] = cmd[4].substr(1); // ':' karakterini çıkar
	}

	client.setUsername(cmd[1]);
	client.setHostName(cmd[2]);
	client.setRealName(cmd[4]);
	client.MsgToClient("Set User");
}

