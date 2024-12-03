#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "../Server.hpp"

void Nick(Client &client, std::string nickname);
void Join(std::map<std::string, Channel> *channels, Client *client, std::string channelName);
void Privmsg(Client client, std::string rawMessage, std::map<std::string, Channel> channels, std::map<int, Client>& clients);

#endif