#include "Server.hpp"
#include "protocolMessages.hpp"
#include <string>

void pass(Server &server, Client &client, std::vector<std::string> cmd)
{
	if (cmd.size() != 2)
	{
		throw ERR_NEEDMOREPARAMS(client.getNickname(), "PASS");
		client.setPass(false);
		return ;
	}
	else if (client.isRegistered() == true)
	{
		throw (ERR_ALREADYREGISTRED(client.getNickname()));
		return ;
	}
	else if (cmd[1] != server.getPassword())
	{
		client.setPass(false);
		throw ERR_PASSWDMISMATCH(client.getNickname());
		return ;
	}
	client.setPass(true);
}
