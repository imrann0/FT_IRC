#include "Channel.hpp"
#include "Client.hpp"
#include "Tools.hpp"
#include <algorithm>

#include <vector>

//	Command: KICK
//	Parameters: <channel> <user> [<comment>]

//	REPL
//	:WiZ KICK #Finnish John
//	Kickleyen kişi  :<nick>!<user>@<host> KICK <channel> <target> :<reason>


//	ERROR:
//		ERR_NEEDMOREPARAMS :
//		ERR_NOSUCHCHANNEL :	<channel name> :No such channel
//		ERR_BADCHANMASK : ?
//		ERR_CHANOPRIVSNEEDED :	<channel> :You're not channel operator
//		ERR_NOTONCHANNEL :	"<channel> :You're not on that channel"
//		ERR_USERNOTINCHANNEL : :<server> 441 <nick> <user> <channel> :They aren't on that channel

#define RPL_KICK(nick, user, host, channel, target ,reason) ":" + nick + "!" + user + "@" + host + " KICK " + channel + " " + target + " :" + reason
#include <iostream>
void	kick(std::map<std::string, Channel> &channels, Client &client, std::vector<std::string> &cmd)
{
	if (cmd.size() <= 2 || cmd.size() > 4)
		throw ERR_NEEDMOREPARAMS(client.getUsername(), cmd[0]);
	std::vector<std::string>	users = split(cmd[2], ',');
	std::vector<std::string>	channelsName = split(cmd[1], ',');
	if (users.size() != 1 || channelsName.size() != 1)
		client.MsgToClient(ERR_NEEDMOREPARAMS(client.getUsername(), cmd[0]));
	else if (channels.find(channelsName[0]) == channels.end())
		std::cout <<  "ERR_NOSUCHCHANNEL" << std::endl;
	else if (channels[channelsName[0]].IsClient(client) == false)
		std::cout <<  "ERR_NOTONCHANNEL" << std::endl;
	else if (channels[channelsName[0]].IsOperator(client) == false)
		std::cout <<  "ERR_CHANOPRIVSNEEDED" << std::endl;
	else if (channels[channelsName[0]].IsClient(users[0]) == false)
		std::cout <<  "ERR_USERNOTINCHANNEL" << std::endl;
	else
	{
		std::string reason = "";
		if (cmd.size() == 4)
			reason = cmd[3];
		std::string message = RPL_KICK(client.getNickname(), client.getUsername(), client.getHostName(),
									channels[channelsName[0]].getName(),
									channels[channelsName[0]].getClient(users[0]).getNickname(),
									reason);
		channels[channelsName[0]].Brodcast(message);
		channels[channelsName[0]].ClientRemove(channels[channelsName[0]].getClient(users[0]));
	}
}
