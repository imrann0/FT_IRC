#include "Client.hpp"
#include "Tools.hpp"
#include <iostream>
#include <cstring>	 	//  strerror
#include <sys/socket.h> // send
#include "Channel.hpp" // RPL and ERR
// :!bdemirbu@0 NICK bdemirbu_


bool isValidNickname(const std::string& nickname) {
    if (nickname.length() < 1 || nickname.length() > 9)
        return false;

    if (!isalpha(nickname[0]))
        return false;

    for (size_t i = 0; i < nickname.length(); ++i)
    {
        char c = nickname[i];
        if (!isalnum(c) && c != '-' && c != '[' && c != ']' && c != '\\' && c != '^' && c != '{' && c != '}' && c != '_')
            return false;
    }
    return true;
}

void Nick(std::map<int, Client>& clients, Client &client, std::vector<std::string> cmd, std::map<std::string, Channel> &channels) // ?
{
	if (cmd.size() == 1)
		throw ERR_NONICKNAMEGIVEN(client.getNickname());
	else if (cmd.size() != 2)
		throw ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0]);
	else if (IsClient(clients, cmd[1]) == true)
		throw ERR_NICKNAMEINUSE(cmd[1]);
	else if (isValidNickname(cmd[1]) == false)
		throw ERR_ERRONEUSNICKNAME(cmd[1]);
	else if (client.isRegistered() == true)
    {
		client.MsgToClient(RPL_NICK(client.getPrefixName(), cmd[1]));
        for (std::map<std::string, Channel>::iterator it; it != channels.end(); it++)
        {
            if (it->second.IsClient(client))
                it->second.Brodcast(RPL_NAMREPLY(client.getPrefixName(), it->second.getName(), it->second.getUsersNames()));
        }
    }
	client.setNickname(cmd[1]);
}


void Nick(std::map<int, Client>& clients, Client &client, std::vector<std::string> cmd) // ?
{
	if (cmd.size() == 1)
		throw ERR_NONICKNAMEGIVEN(client.getNickname());
	else if (cmd.size() != 2)
		throw ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0]);
	else if (IsClient(clients, cmd[1]) == true)
		throw ERR_NICKNAMEINUSE(cmd[1]);
	else if (isValidNickname(cmd[1]) == false)
		throw ERR_ERRONEUSNICKNAME(cmd[1]);
	else if (client.isRegistered() == true)
    {
		client.MsgToClient(RPL_NICK(client.getPrefixName(), cmd[1]));
    }
	client.setNickname(cmd[1]);
}
