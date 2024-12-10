#include <map>
#include <iostream>
#include "Channel.hpp"
#include "Client.hpp"

void    Part(std::map<std::string, Channel> &channels, Client &client,  std::vector<std::string> &cmd)
{
    if (cmd.size() != 3)
        client.MsgToClient(ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0])); // burda yadırılan tüm parametreler olucak cmd.size() kadar
    if (channels.find(cmd[1]) == channels.end())
        client.MsgToClient(ERR_NOSUCHCHANNEL(client.getPrefixName(), cmd[1]));
    else
    {
        if (channels[cmd[1]].IsClient(client))
        {
            std::string mes = RPL_PART(client.getPrefixName(), cmd[1]);
            channels[cmd[1]].ClientRemove(client);
            channels[cmd[1]].Brodcast(mes);
            if (channels[cmd[1]].IsOperator(client))
                channels[cmd[1]].OperatorRemove(client);
            if (channels[cmd[1]].getSizeClient() == "0")
                 channels.erase(cmd[1]);
        }
        else
            client.MsgToClient(ERR_NOTONCHANNEL(client.getNickname(), cmd[1]));
    }
}
