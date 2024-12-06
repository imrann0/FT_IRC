#include "Channel.hpp"
#include "Client.hpp"

//  :Angel INVITE Wiz #Dust 
void Invite(std::map<std::string, Channel> &channels, Client &client, std::vector<std::string> cmd)
{
    if (cmd.size() == 3)
    {
        std::string in = RPL_INVITING(client.getNickname(), cmd[1], cmd[2]);
        channels[cmd[2]].Brodcast(in);
    }
}