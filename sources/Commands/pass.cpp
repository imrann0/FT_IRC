/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemirbu <bdemirbu@student.42kocaeli.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:54:06 by bdemirbu          #+#    #+#             */
/*   Updated: 2024/12/05 18:30:14 by bdemirbu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "Tools.hpp"
#include <string>

void pass(Server &server, Client &client, std::vector<std::string> cmd)
{
	if (cmd.size() != 2 || cmd[1] != server.getPassword())
	{
		client.setPass(false);
		client.MsgToClient("ERROR: Invalid Password");
		return ;
	}
	client.setPass(true);
}
