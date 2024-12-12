#include "Channel.hpp"
#include "Client.hpp"
#include <string>
#include <map>

//:server 352 <nick> <channel> <user> <host> <server> <nick> <H|G>[*][@|+] :<hopcount> <realname>
// <client> <channel> <username> <host> <server> <nick> <flags> :<hopcount> <realname>
// :server 315 <nick> <mask> :End of WHO list

//#define RPL_WHOREPLY(client, channel, username, host, target, status, realname) ": 352 " + client + " " + channel + " " + username + " " + host + " " + target + " " + status + " :0 " + realname
//#define RPL_WHOREPLY(client, channel,username, host,target, status, realname)   ": 352 " + client + " " + channel + " " + username + " " + host + " " + "localhost "  + target +  " H"+status + ":0 " + realname
#define RPL_WHOREPLY(client, channel, username, host, target, status, realname) ": 352 " + client + " " + channel + " " + username + " " + host + " * " + target + " " + status + " :0 " + realname
#define RPL_ENDOFWHO(client, channel) ":* 315 " + client + " " + channel + " :End of WHO list"
void who(std::map<std::string, Channel> &channels, Client client, std::vector<std::string> &cmd)
{
    if (cmd.size() != 2)
        client.MsgToClient(ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0]));
    else if (channels.find(cmd[1]) == channels.end())
        client.MsgToClient(ERR_NOSUCHCHANNEL(client.getPrefixName(), cmd[1]));
    else
    {
        
        std::vector<Client *>::iterator it;
        std::string users;
        for (it = channels[cmd[1]].getClients().begin(); it != channels[cmd[1]].getClients().end(); it++)
        {
            client.MsgToClient(RPL_WHOREPLY(client.getPrefixName(), cmd[1], (*it)->getUsername(), "", "localhost", "H", ""));
        }
        client.MsgToClient(RPL_ENDOFNAMES(client.getPrefixName(), cmd[1]));
    }
}
