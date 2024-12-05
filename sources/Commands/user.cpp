/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemirbu <bdemirbu@student.42kocaeli.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 10:47:10 by bdemirbu          #+#    #+#             */
/*   Updated: 2024/12/05 19:52:39 by bdemirbu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Tools.hpp"
#include "error.hpp"

#include <iostream>
/*
	Command: USER
	Parameters: <username> <hostname> <servername> <realname>
	Numeric Replies:
           ERR_NEEDMOREPARAMS :
		   ERR_ALREADYREGISTRED : "<command> :Not enough parameters"
 */
void	user(Client &client, std::vector<std::string> cmd)
{
	if (cmd.size() != 5)
	{
		client.MsgToClient(ERR_NEEDMOREPARAMS(cmd[0]));
		return;
	}
	if (cmd[4][0] == ':') {
		cmd[4] = cmd[4].substr(1); // ':' karakterini çıkar
	}

	client.setUsername(cmd[1]);
	client.setHostName(cmd[2]);
	client.setRealName(cmd[4]);
	client.MsgToClient("Set User");
}

