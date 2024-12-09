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
void    who(std::map<std::string, Channel> &channels, Client client,  std::vector<std::string> &cmd)
{
    (void)channels;
    (void)client;
    (void)cmd;
    /*if (cmd.size() != 2)
        client.MsgToClient(ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0])); // kalan parametreler eklencek
    else if (channels.find(cmd[1]) == channels.end())
        client.MsgToClient(ERR_NOSUCHCHANNEL(client.getPrefixName(), cmd[1]));
    else
    {
        std::vector<Client *>::iterator user = channels[cmd[1]].getClients().begin();
        for (; user != channels[cmd[1]].getClients().end(); ++user)
        {
            std::string status = channels[cmd[1]].IsOperator(*user[0]) ? "O" : "H";
            client.MsgToClient(RPL_WHOREPLY(client.getNickname(), cmd[1], client.getUsername() ,client.getHostName(), user[0]->getNickname(),status, user[0]->getRealName()));
        }
        client.MsgToClient(RPL_ENDOFWHO(client.getNickname(), cmd[1]));
    }*/
}
