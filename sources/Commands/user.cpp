#include "Client.hpp"
#include <vector>
#include "protocolMessages.hpp"

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
	client.setUsername(cmd[1]);
	client.setHostName(cmd[2]);
	client.setRealName(cmd[4]);
}

