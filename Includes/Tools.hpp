#ifndef TOOLS_HPP
# define TOOLS_HPP

#include "Client.hpp"
#include <map>

void chatRegisterClient(const std::string& message, Client *client);
const Client getClientNameFd(std::map<int, Client>& clients, const std::string& target);

# endif
