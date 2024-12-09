#include "Client.hpp"
#include "Channel.hpp"
#include "Tools.hpp"
#include "Command.hpp"
#include <sys/socket.h> // send
#include <map>

void Privmsg(Client client, std::vector<std::string> &cmd, std::map<std::string, Channel> channels, std::map<int, Client>& clients)
{
    // :Alice!alice@irc.example.com PRIVMSG #general :Herkese merhaba!
    // :<sender>!<user>@<host> PRIVMSG <channel> :<message>
	// :<sender_nickname>!<sender_username>@<sender_host> PRIVMSG <recipient_nickname> :<message> ->özel mesaj
	// Mesajı parçalara ayır (alıcı ve içerik)

	std::string privMessage;
	if (cmd.size() != 3)
	{
		//eksik argüman hatası;
		return ;
	}
	else if (cmd[1][0] == '#')
	{
		if (channels.find(cmd[1]) == channels.end())
			client.MsgToClient(ERR_NOSUCHCHANNEL(client.getPrefixName(), cmd[1]));
		else
		{
			privMessage = RPL_PRIVMSG(client.getPrefixName(), cmd[1], cmd[2]);
			channels[cmd[1]].Brodcast(privMessage);
		}
	}
	else
	{
        try
        {
            Client targetClient = getClientNameFd(clients, cmd[1]);
			privMessage = RPL_PRIVMSG(client.getPrefixName(), targetClient.getNickname(), cmd[2]);
			targetClient.MsgToClient(privMessage);
            //send(clientFd, mes.c_str(), mes.length(), 0);
        }
        catch (const std::exception& e)
        {
			client.MsgToClient("ERROR :User not found");	// ERR_NOSUCHNICK
            return;
        }

	}
}
