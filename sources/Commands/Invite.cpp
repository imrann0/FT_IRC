#include "Channel.hpp"
#include "Client.hpp"
#include "Tools.hpp"
#include <iostream>
void    Invite(std::map<std::string, Channel> &channels, std::map<int, Client> &clients ,Client &client, std::vector<std::string> cmd)
{
    if (cmd.size() == 3)
    {
        if (IsClient(clients, cmd[1]))
        {
            if (channels.find(cmd[2]) == channels.end())
                client.MsgToClient(ERR_NOSUCHCHANNEL(client.getPrefixName(), cmd[2]));
            else
            {
                std::string in = RPL_INVITING(client.getNickname(), cmd[1], cmd[2]);
                channels[cmd[2]].setInvite(cmd[1]);
                client.MsgToClient(in);
            }
        }
        else
            client.MsgToClient(ERR_NOSUCHNICK(client.getNickname(), cmd[1]));
    }
}