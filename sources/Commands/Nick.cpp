#include "Client.hpp"
#include "Tools.hpp"
#include <iostream>
#include <cstring>	 	//  strerror
#include <sys/socket.h> // send

void Nick(Client &client, std::vector<std::string> cmd)
{
	if (cmd.size() == 1)
	{
		std::cout << ":localhost 431 * " << client.getClientFd() << " nickname " << client.getNickname();
	}
	if (client.getNickname().empty())
	{
		client.setNickname(cmd[1]);
		yolla(client.getClientFd(),  "Nick Created\r\n");
		return ;
	}
	std::string a =  ":" + client.getNickname() + "!" + client.getUsername() + "@localhost NICK " + cmd[1] + "\r\n";
	int err = yolla(client.getClientFd(), a);
	if (err < 0)
		std::cerr  << std::strerror(err) << std::endl;
	else
		std::cout << "NICK UPDATE SUCCSES" << std::endl;

	client.setNickname(cmd[1]);
	std::cout << "Client" << client.getClientFd() << " set nickname: " << client.getNickname() << std::endl;
}
