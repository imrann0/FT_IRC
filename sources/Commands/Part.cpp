#include <map>
#include <iostream>
#include "Channel.hpp"
#include "Client.hpp"

void    Part(std::map<std::string, Channel> &channels, Client client,  std::vector<std::string> &cmd)
{
    (void)client;
    (void) channels;
    (void)cmd;
}
