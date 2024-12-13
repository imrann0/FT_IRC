#include <map>
#include "Channel.hpp"
#include "Client.hpp"
#include "protocolMessages.hpp"

void    Part(std::map<std::string, Channel> &channels, Client &client,  std::vector<std::string> &cmd)
{
    if (cmd.size() != 3)
        client.MsgToClient(ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0]));
    if (channels.find(cmd[1]) == channels.end())
        client.MsgToClient(ERR_NOSUCHCHANNEL(client.getPrefixName(), cmd[1]));
    else
    {
        if (channels[cmd[1]].IsClient(client))
        {
            channels[cmd[1]].Brodcast(RPL_PART(client.getPrefixName(), cmd[1]));
            if (channels[cmd[1]].IsOperator(client))
                channels[cmd[1]].OperatorRemove(client);
            channels[cmd[1]].ClientRemove(client);
            if (channels[cmd[1]].getSizeClient() == "0")
                channels.erase(cmd[1]);
        }
        else
            client.MsgToClient(ERR_NOTONCHANNEL(client.getNickname(), cmd[1]));
    }
}
