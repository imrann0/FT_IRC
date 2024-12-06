#include "Command.hpp"
#include <vector>

void    Topic(Channel &channel, Client &client, std::vector<std::string> cmd)
{
    if (channel.IsFlags('t') == false)
    {
        std::cout << channel.getTopic().empty() << " " << cmd.size() << std::endl;
        std::string topic;
        if (cmd.size()  == 3)
        {
            topic = RPL_TOPIC(client.getNickname(), channel.getName(), cmd[2]);
            channel.TopicAdd(topic);
        }
        else if (!channel.getTopic().empty())
            topic = RPL_TOPIC(client.getNickname(), channel.getName(), channel.getTopic());
        else
            client.MsgToClient(RPL_NOTOPIC(client.getNickname(), channel.getName()));
        channel.Brodcast(topic);
    }
    else
    {
        if (channel.IsOperator(client))
        {
            std::string topic;
            if (cmd.size()  == 3)
            {
                topic = RPL_TOPIC(client.getNickname(), channel.getName(), cmd[2]);
                channel.TopicAdd(topic);
            }
            else if (!channel.getTopic().empty())
                topic = RPL_TOPIC(client.getNickname(), channel.getName(), channel.getTopic());
            else
                client.MsgToClient(RPL_NOTOPIC(client.getNickname(), channel.getName()));
        }
        else
            client.MsgToClient(ERR_CHANOPRIVSNEEDED(client.getNickname(), channel.getName()));
    }

}