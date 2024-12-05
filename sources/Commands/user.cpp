/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemirbu <bdemirbu@student.42kocaeli.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 10:47:10 by bdemirbu          #+#    #+#             */
/*   Updated: 2024/12/04 19:49:16 by bdemirbu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Tools.hpp"

#include <iostream>

void	user(Client &client, std::string str)
{
	// Command: USER
	// Parameters: <username> <hostname> <servername> <realname>
	size_t firstSpace = str.find(" "); // ilk boşluğu bul
	size_t secondSpace = str.find(" ", firstSpace + 1); // ikinci boşluğu bul
	size_t thirdSpace = str.find(" ", secondSpace + 1); // üçüncü boşluğu bul

	// Parçaları ayıralım
	std::string username = str.substr(0, firstSpace); // username
	std::string hostname = str.substr(firstSpace + 1, secondSpace - firstSpace - 1); // hostname
	std::string servername = str.substr(secondSpace + 1, thirdSpace - secondSpace - 1); // servername
	std::string realname = str.substr(thirdSpace + 1); // realname kısmını almak için ':'

	if (realname[0] == ':') {
		realname = realname.substr(1); // ':' karakterini çıkar
	}
	std::cout << "isimler" << std::endl;
	std::cout << username << std::endl;
	std::cout << hostname << std::endl;
	std::cout << servername << std::endl;
	std::cout << realname << std::endl;
	// Client bilgilerini güncelle
	client.setUsername(username);
	client.setRealName(realname);
	client.setHostName(hostname);
	yolla(client.getClientFd(),  "User Created\r\n");
}

