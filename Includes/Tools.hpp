#ifndef TOOLS_HPP
# define TOOLS_HPP

#include "Client.hpp"
#include <map>
#include <vector>

void						chatRegisterClient(const std::string& message, Client *client);
const Client				getClientNameFd(std::map<int, Client>& clients, const std::string& target);
int							yolla(int fd, std::string str);
std::vector<std::string>	split(std::string str, char	c);

# endif
