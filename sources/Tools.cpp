#include "Tools.hpp"
#include "Channel.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

std::vector<std::string>	split(std::string str, char	c)
{
	std::vector<std::string>	ret;
	unsigned long				end;
	unsigned long				start;
	std::string					findStr;

	if (str.find_first_not_of(c) == std::string::npos)
		return ret;
	end = str.find(c);
	start = 0;
	while (end != std::string::npos)
	{
		findStr = str.substr(start, end - start);
		if (findStr[0] == ':') // "," ile ayırmada sıkıntı
			break ;
		if (!findStr.empty())
			ret.push_back(findStr);
		start = end + 1;
		end = str.find(c, start);
	}
	findStr = str.substr(start);
	if (findStr[0] == ':') // "," ile ayırmada sıkıntı
		findStr = str.substr(start + 1);
	if (!findStr.empty())
		ret.push_back(findStr);
	return (ret);
}

const Client getClientNameFd(const std::map<int, Client>& clients, const std::string& target)
{
    for (std::map<int, Client>::const_iterator user = clients.begin(); user != clients.end(); ++user)
    {
        if (user->second.getNickname() == target)
            return user->second;
    }
    throw std::runtime_error("Client not found");
}

bool IsClient(const std::map<int, Client>& clients, const std::string& client)
{
    try
    {
        getClientNameFd(clients, client);
        return true;
    }
    catch (const std::exception& e)
    {
        return false;
    }
}

void RemoveChannels(std::map<std::string, Channel> &channels, Client &client)
{
    std::map<std::string, Channel>::iterator it = channels.begin();

    while (it != channels.end())
    {
        if (it->second.IsClient(client))
        {
            if (it->second.IsOperator(client))
                it->second.OperatorRemove(client);

            it->second.ClientRemove(client);

            if (it->second.getSizeClient() == "0")
            {
                std::map<std::string, Channel>::iterator temp = it;
                ++it;
                channels.erase(temp);
                continue;
            }
            else
            {

                std::string nameReplyMessage = RPL_NAMREPLY(client.getPrefixName(), it->second.getName(), it->second.getUsersNames());
                std::string endOfNamesMessage = RPL_ENDOFNAMES(client.getPrefixName(), it->second.getName());
                it->second.Brodcast(nameReplyMessage);
                it->second.Brodcast(endOfNamesMessage);
            }
        }
        ++it;
    }
}

