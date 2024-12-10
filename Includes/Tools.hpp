#ifndef TOOLS_HPP
# define TOOLS_HPP

#include "Client.hpp"
#include <map>
#include <vector>

const Client                getClientNameFd(const std::map<int, Client>& clients, const std::string& target);
bool	                    IsClient(const std::map<int, Client>& clients, const std::string &client);
std::vector<std::string>	split(std::string str, char	c);

# endif
