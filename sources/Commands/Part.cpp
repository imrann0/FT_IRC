#include <map>
#include <iostream>

#include "Channel.hpp"
#include "Client.hpp"



void    Part(std::map<std::string, Channel> &channels, Client client, std::string str)
{
    (void)client;
    (void) channels;
    size_t pos = str.find(" ");
    std::string channel = str.substr(0, pos);
    channels[channel].getClients();
}
