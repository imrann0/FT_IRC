#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "Server.hpp"

void	Nick(Client &client, std::vector<std::string> cmd);
void	Join(std::map<std::string, Channel> &channels, Client &client, std::vector<std::string> &cmd);
void	Privmsg(Client client, std::string rawMessage, std::map<std::string, Channel> channels, std::map<int, Client>& clients);
void    Quit(int clientFd, std::map<int, Client>& clients, std::vector<pollfd>& pollFds);
void	user(Client &client, std::vector<std::string> cmd);
void    Part(std::map<std::string, Channel> &channels, Client client, std::string str);
void	pass(Server &server, Client &client, std::vector<std::string> cmd);
void    Mode(std::map<std::string, Channel> &channles, Client &client, std::string str);

#endif
