/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemirbu <bdemirbu@student.42kocaeli.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 10:47:10 by bdemirbu          #+#    #+#             */
/*   Updated: 2024/12/04 14:24:07 by bdemirbu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void	user(Client &client, std::string str)
{

	size_t firstSpace = str.find(" "); // ilk boşluğu bul
	size_t secondSpace = str.find(" ", firstSpace + 1); // ikinci boşluğu bul
	size_t thirdSpace = str.find(" ", secondSpace + 1); // üçüncü boşluğu bul

	// Parçaları ayıralım
	std::string username = str.substr(0, firstSpace); // username
	std::string hostname = str.substr(firstSpace + 1, secondSpace - firstSpace - 1); // hostname
	std::string servername = str.substr(secondSpace + 1, thirdSpace - secondSpace - 1); // servername
	std::string realname = str.substr(thirdSpace + 2); // realname kısmını almak için ':'

	if (realname[0] == ':') {
		realname = realname.substr(1); // ':' karakterini çıkar
	}

	// Client bilgilerini güncelle
	client.setUsername(username);
	client.setRealName(realname);
	client.setHostName(hostname);
}
