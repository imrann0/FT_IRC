#include "Client.hpp"
#include "Channel.hpp"
#include "Tools.hpp"
#include "Command.hpp"
#include <sys/socket.h> // send
#include <map>

void Privmsg(Client client, std::vector<std::string> &cmd, std::map<std::string, Channel> channels, std::map<int, Client>& clients)
{
	std::string privMessage;
	if (cmd.size() == 1)
	{
		throw ERR_NORECIPIENT(client.getNickname());
	}
	else if (cmd[1].find(',') != std::string::npos)
		throw ERR_TOOMANYTARGETS(client.getNickname());
	else if (cmd.size() == 2)
		throw ERR_NOTEXTTOSEND(client.getNickname());
	else if (cmd[1][0] == '#')
	{
		if (channels.find(cmd[1]) == channels.end())
			throw (ERR_NOTOPLEVEL(client.getNickname()));
		else
		{
			privMessage = RPL_PRIVMSG(client.getPrefixName(), cmd[1], cmd[2]);
			channels[cmd[1]].Brodcast(privMessage, client);
		}
	}
	else
	{
        try
        {
            Client targetClient = getClientNameFd(clients, cmd[1]);
			privMessage = RPL_PRIVMSG(client.getPrefixName(), targetClient.getNickname(), cmd[2]);
			targetClient.MsgToClient(privMessage);
        }
        catch (const std::exception& e)
        {
			client.MsgToClient(ERR_NOSUCHNICK(client.getNickname(), cmd[1]));
            return;
        }

	}
}
