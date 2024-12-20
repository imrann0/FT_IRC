#include "Channel.hpp"
#include "Client.hpp"
#include "Tools.hpp"
#include "protocolMessages.hpp"

#include <vector>

//	Command: KICK
//	Parameters: <channel> <user> [<comment>]

//	REPL
//	:WiZ KICK #Finnish John

//	ERROR:
//		ERR_NEEDMOREPARAMS :
//		ERR_NOSUCHCHANNEL :	<channel name> :No such channel
//		ERR_BADCHANMASK : ?
//		ERR_CHANOPRIVSNEEDED :	<channel> :You're not channel operator
//		ERR_NOTONCHANNEL :	"<channel> :You're not on that channel"
//		ERR_USERNOTINCHANNEL : :<server> 441 <nick> <user> <channel> :They aren't on that channel

void	kick(std::map<std::string, Channel> &channels, Client &client, std::vector<std::string> &cmd) // +
{
	if (cmd.size() <= 2 || cmd.size() > 4)
		throw ERR_NEEDMOREPARAMS(client.getUsername(), cmd[0]); // +
	std::vector<std::string>	users = split(cmd[2], ',');
	std::vector<std::string>	channelsName = split(cmd[1], ',');
	if (users.size() != 1 || channelsName.size() != 1)
		throw ERR_NEEDMOREPARAMS(client.getUsername(), cmd[0]); // +
	else if (channels.find(channelsName[0]) == channels.end())
		throw ERR_NOSUCHCHANNEL(client.getUsername(), channelsName[0]); // +
	else if (channels[channelsName[0]].IsClient(client) == false)
		throw ERR_NOTONCHANNEL(client.getUsername(), channelsName[0]); // +
	else if (channels[channelsName[0]].IsOperator(client) == false)
		throw ERR_CHANOPRIVSNEEDED(client.getUsername(), channelsName[0]);
	else if (channels[channelsName[0]].IsClient(users[0]) == false)
		throw ERR_NOTONCHANNEL(client.getUsername(), channelsName[0]); // +
	else
	{
		if (cmd[2] != client.getNickname())
		{
			std::string reason = cmd.size() != 4 ? "" : cmd[3];
			std::string message = RPL_KICK(client.getNickname(), client.getUsername(), client.getHostName(),
										channels[channelsName[0]].getName(),
										channels[channelsName[0]].getClient(users[0]).getNickname(),
										reason);
			channels[channelsName[0]].Brodcast(message);
			channels[channelsName[0]].ClientRemove(channels[channelsName[0]].getClient(users[0]));

		}
		else
		{
			std::string m = "You cannot assign this to yourself.";
			client.MsgToClient(m);
		}
	}
}
