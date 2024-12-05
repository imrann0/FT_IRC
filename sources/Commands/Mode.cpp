#include <Channel.hpp>
#include <Tools.hpp>
#include <iostream>
#include <map>

void    o(Channel &channel, Client &client, std::string channelName, std::string &str)
{
    if (channel.IsOperator(client))
    {
        try
        {
            size_t pos = str.find(" ");
            std::string clientName = str.substr(pos + 1);
            Client newOperator = channel.getClient(clientName);
            std::cout << "bune "<< clientName << "$" << channelName << "$\n"; 
            if (str[0] == '+')
            {
                newOperator.MsgToClient(RPL_MODE(client.getPrefixName(), channelName, "+o ", clientName));
                channel.OperatorAdd(newOperator);
                std::string nameReplyMessage = RPL_NAMREPLY(client.getPrefixName(), channelName, channel.getUsersNames());
                std::string endOfNamesMessage = RPL_ENDOFNAMES(client.getPrefixName(), channelName);
                channel.Brodcast(nameReplyMessage);
                channel.Brodcast(endOfNamesMessage);
                return ;
            }
            else if (str[0] == '-')
            {
                newOperator.MsgToClient(RPL_MODE(client.getPrefixName(), channelName, "-o ", clientName));
                channel.OperatorRemove(client);
                std::string nameReplyMessage = RPL_NAMREPLY(client.getPrefixName(), channelName, channel.getUsersNames());
                std::string endOfNamesMessage = RPL_ENDOFNAMES(client.getPrefixName(), channelName);
                channel.Brodcast(nameReplyMessage);
                channel.Brodcast(endOfNamesMessage);
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
    if (channles.find(channelName) == channles.end())
    {
        try
        {
            client.MsgToClient(ERR_NOSUCHCHANNEL(client.getPrefixName(), channelName));
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        

    }
    if (str.compare(1, 1, "o") == 0)
        o(channles[channelName], client, channelName, str);
}