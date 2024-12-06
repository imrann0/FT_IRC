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

	if (cmd.size() != 3)
	{
		//eksik argüman hatası;
		return ;
	}
	if (cmd[1][0] == '#')
	{
		std::vector<Client> users = channels[cmd[1]].getClients();

		for (it user = users.begin(); user != users.end(); user++)
		{
			if (user->getClientFd() == client.getClientFd())
				continue ;
			std::string message = ":" + client.getNickname() + "!" + user->getUsername() + "@" + user->getHostName() + " " + cmd[2] + "\r\n";
			user->MsgToClient(message);
		}
	}
	else
	{
        try
        {
            Client targetClient = getClientNameFd(clients, cmd[1]);
		    std::string mes = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + client.getHostName() + " PRIVMSG " + targetClient.getNickname() + " :" + cmd[2] + "\r\n";
			yolla(targetClient.getClientFd(), mes);
            //send(clientFd, mes.c_str(), mes.length(), 0);
        }
        catch (const std::exception& e)
        {
			client.MsgToClient("ERROR :User not found");	// ERR_NOSUCHNICK
            return;
        }

	}
}
