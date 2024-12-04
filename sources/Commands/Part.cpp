#include <map>
#include <iostream>
#include "Channel.hpp"
#include "Client.hpp"


void    Part(std::map<std::string, Channel> &channels, Client client, std::string str)
{
    (void)client;
    (void) channels;
    try
    {
        size_t pos = str.find(" ");
        std::string channel = str.substr(0, pos);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

    
}
