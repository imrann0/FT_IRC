#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "Server.hpp"

void	Nick(std::map<int, Client>& clients, Client &client, std::vector<std::string> cmd);
void	Join(std::map<std::string, Channel> &channels, Client &client, std::vector<std::string> &cmd);
void	Privmsg(Client client, std::vector<std::string> &cmd, std::map<std::string, Channel> channels, std::map<int, Client>& clients);
void    Quit(std::map<std::string, Channel> channels, std::map<int, Client>& clients, int clientFd, std::vector<pollfd>& pollFds);
void	user(Client &client, std::vector<std::string> cmd);
void    Part(std::map<std::string, Channel> &channels, Client &client,  std::vector<std::string> &cmd);
void	pass(Server &server, Client &client, std::vector<std::string> cmd);
void    Mode(std::map<std::string, Channel> &channles, Client &client ,std::vector<std::string> cmd);
void    Topic(std::map<std::string, Channel> &channels, Client &client, std::vector<std::string> cmd);
void    Invite(std::map<std::string, Channel> &channels, std::map<int, Client> &clients, Client &client, std::vector<std::string> cmd);
void	kick(std::map<std::string, Channel> &channels, Client &client, std::vector<std::string> &cmd);
void    who(std::map<std::string, Channel> &channels, Client client,  std::vector<std::string> &cmd);

#endif
