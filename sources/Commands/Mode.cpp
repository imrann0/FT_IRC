#include "protocolMessages.hpp"

#include <Channel.hpp>
#include <iostream>
#include <cstdlib>
#include <map>

void    o(Channel &channel, Client &client, std::vector<std::string> &cmd)
{

    try
    {
        Client &newOperator = channel.getClient(cmd[3]);
        if (cmd[2][0] == '+' && cmd.size() == 4)
        {
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

void t(Channel &channel, Client &client, std::vector<std::string> &cmd)
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

void    l(Channel &channel, Client &client, std::vector<std::string> cmd)
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

void    k(Channel &channel, Client &client, std::vector<std::string> cmd)
{
    if (cmd.size() > 4 || cmd.size() <= 2)
        throw ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0]);
    if (cmd[2][0] == '-')
    {
        std::string mes = RPL_MODEONE(client.getPrefixName(), cmd[1], cmd[2]);
        channel.Brodcast(mes);
        channel.setFlags('k', false);
        channel.setPassword("");
    }
    else if (cmd.size() == 4)
    {
        std::string mes = RPL_MODE(client.getPrefixName(), cmd[1], cmd[2], cmd[3]);
        channel.Brodcast(mes);
        channel.setFlags('k', true);
        channel.setPassword(cmd[3]);
    }
    else
        throw ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0]);
}

void i(Channel &channel, Client &client, std::vector<std::string> cmd)
{

    std::string modeMessage;
    if (cmd[2][0] == '+')
    {
        if (cmd.size() == 4)
        {
            modeMessage = RPL_MODE(client.getPrefixName(), cmd[1], "+i", cmd[3]);
            channel.setFlags('i', true);
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
            try
            {
                modeMessage =  RPL_MODE(client.getPrefixName(), cmd[1], "-i", cmd[3]);
                channel.removeInvite(cmd[3]);
                channel.Brodcast(modeMessage);
            }
            catch(const std::exception& e)
            {
                client.MsgToClient(ERR_NOTONCHANNEL(client.getNickname(), cmd[1]));
            }
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

void Mode(std::map<std::string, Channel> &channles, Client &client ,std::vector<std::string> cmd)
{
    if (cmd.size() == 1)
        throw ERR_NEEDMOREPARAMS(client.getNickname(), cmd[0]); // +
    else if (channles.find(cmd[1]) == channles.end())
        throw ERR_NOSUCHCHANNEL(client.getPrefixName(), cmd[1]); // +
    else if (cmd.size() == 2)
        client.MsgToClient(RPL_CHANNELMODEIS(client.getPrefixName(), cmd[1], channles[cmd[1]].getFlags()));
    else if (channles[cmd[1]].IsClient(client) == false)
        throw ERR_NOTONCHANNEL(client.getNickname(), cmd[1]); // +
    else if (channles[cmd[1]].IsOperator(client) == false)
        throw ERR_CHANOPRIVSNEEDED(client.getNickname(), cmd[1]); // ? mesaj kanalda gözüküyor
    else if (cmd.size() > 2)
    {
        if (cmd[2] == "+o" || cmd[2] == "-o")
            o(channles[cmd[1]], client, cmd);
        else if (cmd[2] == "+t" || cmd[2] == "-t")
            t(channles[cmd[1]], client, cmd);
        else if (cmd[2] == "+l" || cmd[2] == "-l")
            l(channles[cmd[1]], client, cmd);
        else if (cmd[2] == "+i" || cmd[2] == "-i")
            i(channles[cmd[1]], client, cmd);
        else if (cmd[2] == "+k" || cmd[2] == "-k")
            k(channles[cmd[1]], client, cmd);
        else
            throw ERR_UNKNOWNMODE(client.getNickname(), cmd[1], cmd[2]); // +
    }
}
