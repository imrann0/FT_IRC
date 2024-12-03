#include "../Client.hpp"
#include "../Channel.hpp"
#include "../Tools.hpp"
#include <sys/socket.h> // send
#include <map>


void Privmsg(Client client, std::string rawMessage, std::map<std::string, Channel> channels, std::map<int, Client>& clients)
{
    // :Alice!alice@irc.example.com PRIVMSG #general :Herkese merhaba!
    // :<sender>!<user>@<host> PRIVMSG <channel> :<message>
	// :<sender_nickname>!<sender_username>@<sender_host> PRIVMSG <recipient_nickname> :<message> ->özel mesaj
	rawMessage.erase(rawMessage.find_last_not_of("\r\n") + 1);

	// Mesajı parçalara ayır (alıcı ve içerik)
	size_t firstSpace = rawMessage.find(" ");
	size_t secondSpace = rawMessage.find(" ", firstSpace + 1);

	if (firstSpace == std::string::npos || secondSpace == std::string::npos) {
		// Geçersiz format
		std::string error = "ERROR :Invalid PRIVMSG format\r\n";
		send(client.getClientFd(), error.c_str(), error.length(), 0);
		return;
	}

	std::string target = rawMessage.substr(firstSpace + 1, secondSpace - firstSpace - 1); // Alıcı (#channel veya kullanıcı)
	std::string content = rawMessage.substr(secondSpace + 1); // Mesaj içeriği
	if (target[0] == '#')
	{
		std::vector<Client> users = channels[target].getClients();
		
		for (it user = users.begin(); user != users.end(); user++)
		{
			if (user->getClientFd() == client.getClientFd())
				continue ;
			int fda = user->getClientFd();
			std::string m = ":" + client.getNickname() + "!" + user->getUsername() + "@" + user->getHostName() + " "+ rawMessage + "\r\n";
			send(fda, m.c_str(), m.length(), 0);
		}
	}
	else
	{
        try
        {
            Client targetClient = getClientNameFd(clients, target);
            if (content[0] == ':')
                content.erase(0, 1);
		    std::string mes = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + client.getHostName() + " PRIVMSG " + targetClient.getNickname() + " :" + content + "\r\n";
		    send(targetClient.getClientFd(), mes.c_str(), mes.length(), 0);
            //send(clientFd, mes.c_str(), mes.length(), 0);
        } 
        catch (const std::exception& e)
        {
            std::string error = "ERROR :User not found\r\n";
            send(client.getClientFd(), error.c_str(), error.length(), 0);
            return;
        }

	}
}