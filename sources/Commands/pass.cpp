/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemirbu <bdemirbu@student.42kocaeli.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:54:06 by bdemirbu          #+#    #+#             */
/*   Updated: 2024/12/04 17:54:34 by bdemirbu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "Tools.hpp"
#include <string>

void pass(Server &server, Client &client, std::string str)
{
	std::cout << "girdi: |"<<str << "|"
	<< "\nşifre: |"<< server.getPassword() << "\n|\n";
	if (server.getPassword() == str)
		client.setPass(true);
	else
	{
		client.setPass(false);
		yolla(client.getClientFd(), "ERROR: Invalid Password");

	}
}
