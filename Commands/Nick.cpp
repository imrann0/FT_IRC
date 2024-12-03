#include "../Client.hpp"
#include <iostream>
#include <cstring>	 	//  strerror
#include <sys/socket.h> // send

void Nick(Client &client, std::string nickname)
{
	nickname.erase(nickname.find_last_not_of("\r\n") + 1);
	if (nickname.length() == 0)
	{
		std::cout << ":localhost 431 * " << client.getClientFd() << " nickname " << client.getNickname();
	}
	if (client.getNickname().empty())
	{
		client.setNickname(nickname);
		return ;
	}
	std::string a =  ":" + client.getNickname() + "!" + client.getUsername() + "@localhost NICK " + nickname + "\r\n";
	int err = send(client.getClientFd(), a.c_str(), a.length(), 0);
	if (err < 0)
		std::cerr  << std::strerror(err) << std::endl;
	else
		std::cout << "NICK UPDATE SUCCSES" << std::endl;

	client.setNickname(nickname);
	std::cout << "Client" << client.getClientFd() << " set nickname: " << client.getNickname() << std::endl;
}
