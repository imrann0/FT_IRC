#include <Channel.hpp>
#include <Tools.hpp>
#include <iostream>
#include <map>

void    o(Channel &channel, Client &client, std::string str)
{
    if (channel.IsOperator(client))
    {
        try
        {
            size_t pos = str.find(" ");
            std::string clientName = str.substr(pos + 1);
            Client newOperator = channel.getClient(clientName);
            if (str[0] == '+')
            {
                channel.OperatorAdd(newOperator);
                newOperator.MsgToClient(RPL_MODE(client.getPrefixName(), str.substr(pos), "+o ", clientName));
                return ;
            }
            else if (str[0] == '-')
            {
                
                newOperator.MsgToClient(RPL_MODE(client.getPrefixName(), str.substr(pos), "-o ", clientName));
                return ;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        

    }
}

void Mode(std::map<std::string, Channel> &channles, Client &client, std::string str)
{
    size_t pos = str.find(" ");
    std::string channelName = str.substr(0, pos);
    str = str.substr(pos + 1);
    if (str.compare(1, 1, "o") == 0)
        o(channles[channelName], client, str);
}