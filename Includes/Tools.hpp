#ifndef TOOLS_HPP
# define TOOLS_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include <map>
#include <vector>

const Client                getClientNameFd(const std::map<int, Client>& clients, const std::string& target);
bool	                    IsClient(const std::map<int, Client>& clients, const std::string &client);
bool                        IsClient(const std::map<int, Client>& clients, Client &client);
std::vector<std::string>	split(std::string str, char	c);
void                        RemoveChannels(std::map<std::string, Channel> &channels, Client &client);
# endif
