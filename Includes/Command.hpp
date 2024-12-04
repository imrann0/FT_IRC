#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "Server.hpp"

void	Nick(Client &client, std::string nickname);
void	Join(std::map<std::string, Channel> &channels, Client &client, std::string channelName);
void	Privmsg(Client client, std::string rawMessage, std::map<std::string, Channel> channels, std::map<int, Client>& clients);
void    Quit(int clientFd, std::map<int, Client>& clients, std::vector<pollfd>& pollFds);
void	user(Client &client, std::string str);
void    Part(std::map<std::string, Channel> &channels, Client client, std::string str);
int		yolla(int fd, std::string str);

#endif
