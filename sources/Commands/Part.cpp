#include <map>
#include <iostream>
#include "Channel.hpp"
#include "Client.hpp"

void    Part(std::map<std::string, Channel> &channels, Client client,  std::vector<std::string> &cmd)
{
    (void)client;
    (void) channels;
    (void)cmd;

    if (cmd.size() != 3)
        client.MsgToClient(ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0])); // burda yadırılan tüm parametreler olucak cmd.size() kadar
    if (channels.find(cmd[1]) == channels.end())
        client.MsgToClient(ERR_NOSUCHCHANNEL(client.getPrefixName(), cmd[1]));
    else
    {
        if (channels[cmd[1]].IsClient(client))
        {
            channels[cmd[1]].ClientRemove(client);
            if (channels[cmd[1]].IsOperator(client))
                channels[cmd[1]].OperatorRemove(client);
        }
        else
            ERR_NOTONCHANNEL(client.getNickname(), cmd[1]);
    }
}
