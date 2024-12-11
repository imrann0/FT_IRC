#include "Command.hpp"
#include <vector>

void    Topic(std::map<std::string, Channel> &channels, Client &client, std::vector<std::string> cmd)
{
    if (cmd.size() == 1)
        throw ERR_NEEDMOREPARAMS(client.getNickname(), "TOPIC"); // +
    else if (channels[cmd[1]].IsClient(client) == false)
        throw ERR_NOTONCHANNEL(client.getNickname(), channels[cmd[1]].getName()); // +
    else if (channels[cmd[1]].IsFlags('t') == false) // herkes değiştirebilir
    {
        std::string topic;
        if (cmd.size()  == 3)
        {
            topic = RPL_TOPIC(client.getNickname(), channels[cmd[1]].getName(), cmd[2]);
            channels[cmd[1]].TopicAdd(cmd[2]);
        }
        else if (!channels[cmd[1]].getTopic().empty())
            topic = RPL_TOPIC(client.getNickname(), channels[cmd[1]].getName(), channels[cmd[1]].getTopic());
        else
            client.MsgToClient(RPL_NOTOPIC(client.getNickname(), channels[cmd[1]].getName()));
        channels[cmd[1]].Brodcast(topic);
    }
    else // sadece operator değiştirebilir
    {
        std::cout << "31" << std::endl;
        if (channels[cmd[1]].IsOperator(client))
        {
            std::cout << "1" << std::endl;
            std::string topic;
            if (cmd.size()  == 3)
            {
                topic = RPL_TOPIC(client.getNickname(), channels[cmd[1]].getName(), cmd[2]);
                channels[cmd[1]].TopicAdd(cmd[2]);
            }
            else if (!channels[cmd[1]].getTopic().empty())
                topic = RPL_TOPIC(client.getNickname(), channels[cmd[1]].getName(), channels[cmd[1]].getTopic());
            else
                client.MsgToClient(RPL_NOTOPIC(client.getNickname(), channels[cmd[1]].getName()));

            if (topic.empty() == false)
                channels[cmd[1]].Brodcast(topic);
        }
        else
            throw ERR_CHANOPRIVSNEEDED(client.getNickname(), channels[cmd[1]].getName()); // +
    }
}