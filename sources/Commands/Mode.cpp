#include <Channel.hpp>
#include <Tools.hpp>
#include <iostream>
#include <cstdlib>
#include <map>

void    o(Channel &channel, Client &client, std::vector<std::string> &cmd)
{
    if (channel.IsOperator(client))
    {
        try
        {
            Client newOperator; 
            if (cmd[2][0] == '+' && cmd.size() == 4)
            {
                newOperator =  channel.getClient(cmd[3]);
                newOperator.MsgToClient(RPL_MODE(client.getPrefixName(), cmd[1], "+o ", cmd[3]));
                channel.OperatorAdd(newOperator);
                std::string nameReplyMessage = RPL_NAMREPLY(client.getPrefixName(), cmd[1], channel.getUsersNames());
                std::string endOfNamesMessage = RPL_ENDOFNAMES(client.getPrefixName(), cmd[1]);
                channel.Brodcast(nameReplyMessage);
                channel.Brodcast(endOfNamesMessage);
                return ;
            }
            else if (cmd[2][0] == '-' && cmd.size() == 4)
            {
                newOperator =  channel.getClient(cmd[3]);
                newOperator.MsgToClient(RPL_MODE(client.getPrefixName(), cmd[1], "-o ", cmd[3]));
                channel.OperatorRemove(newOperator);
                std::string nameReplyMessage = RPL_NAMREPLY(client.getPrefixName(), cmd[1], channel.getUsersNames());
                std::string endOfNamesMessage = RPL_ENDOFNAMES(client.getPrefixName(), cmd[1]);
                channel.Brodcast(nameReplyMessage);
                channel.Brodcast(endOfNamesMessage);
                return ;
            }
            else
                client.MsgToClient(ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0]));
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

void t(Channel &channel, Client &client, std::vector<std::string> &cmd)
{
    if (channel.IsOperator(client))
    {
        std::string modeMessage;
        if (cmd[2][0] == '+' && cmd.size() == 3)
        {
            modeMessage = RPL_MODEONE(client.getPrefixName(), cmd[1], "+t");
            channel.setFlags('t', true);
            channel.Brodcast(modeMessage);
        }
        else if (cmd[2][0] == '-' && cmd.size() == 3)
        {
            modeMessage =  RPL_MODEONE(client.getPrefixName(), cmd[1], "-t");
            channel.setFlags('t', false);
            channel.Brodcast(modeMessage);
        }
        else
           client.MsgToClient(ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0]));
    }
    else
        client.MsgToClient(ERR_CHANOPRIVSNEEDED(client.getNickname(), channel.getName()));
}

void    l(Channel &channel, Client &client, std::vector<std::string> cmd)
{
    if (channel.IsOperator(client))
    {
        std::string modeMessage;
        if (cmd[2][0] == '+' && cmd.size() == 4)
        {
            modeMessage = RPL_MODE(client.getPrefixName(), cmd[1], "+l", cmd[3]);
            channel.setFlags('l', true);
            channel.setLimit(std::atoi(cmd[3].c_str()));
            channel.Brodcast(modeMessage);
        }
        else if (cmd[2][0] == '-' && cmd.size() == 3)
        {
            modeMessage =  RPL_MODEONE(client.getPrefixName(), cmd[1], "-l");
            channel.setFlags('l', false);
            channel.Brodcast(modeMessage);
        }
        else
            client.MsgToClient(ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0]));
    }
    else
        client.MsgToClient(ERR_CHANOPRIVSNEEDED(client.getNickname(), channel.getName()));
}

void i(Channel &channel, Client &client, std::vector<std::string> cmd)
{
    if (channel.IsOperator(client))
    {
        std::string modeMessage;
        if (cmd[2][0] == '+')
        {
            if (cmd.size() == 4)
            {
                modeMessage = RPL_MODE(client.getPrefixName(), cmd[1], "+i", cmd[3]);
                channel.setFlags('l', true);
                channel.setInvite(cmd[3]);
                channel.Brodcast(modeMessage);
            }
            else if (cmd.size() == 3)
            {
                modeMessage = RPL_MODEONE(client.getPrefixName(), cmd[1], "+i");
                channel.setFlags('i', true);
                channel.Brodcast(modeMessage);
            };

        }
        else if (cmd[2][0] == '-')
        {
            if (cmd.size() == 4)
            {
                modeMessage =  RPL_MODE(client.getPrefixName(), cmd[1], "-i", cmd[3]);
                channel.removeInvite(cmd[3]);
                channel.Brodcast(modeMessage);
            }
            else if (cmd.size() == 3)
            {
                modeMessage = RPL_MODEONE(client.getPrefixName(), cmd[1], "-i");
                channel.setFlags('i', false);
                channel.Brodcast(modeMessage);
            };
        }
        else
            client.MsgToClient(ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0]));
    }
    else
        client.MsgToClient(ERR_CHANOPRIVSNEEDED(client.getNickname(), channel.getName()));
}

void Mode(std::map<std::string, Channel> &channles, Client &client ,std::vector<std::string> cmd)
{
    (void)channles;
    (void)client;
    if (channles.find(cmd[1]) == channles.end())
    {
        try
        {
            client.MsgToClient(ERR_NOSUCHCHANNEL(client.getPrefixName(), cmd[1]));
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return ;
        }
    }
    if (cmd.size() > 2)
    {
        if (cmd[2].compare(1, 1, "o") == 0)
            o(channles[cmd[1]], client, cmd);
        else if (cmd[2].compare(1, 1, "t") == 0)
            t(channles[cmd[1]], client, cmd);
        else if (cmd[2].compare(1, 1, "l") == 0)
            l(channles[cmd[1]], client, cmd);
        else if (cmd[2].compare(1, 1, "i") == 0)
            i(channles[cmd[1]], client, cmd);
    }

}
