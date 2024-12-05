#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include "Client.hpp"

typedef std::vector<Client>::iterator it;

#define RPL_NAMREPLY(source, channel, users)            ": 353 " + source + " = " + channel + " :" + users
#define RPL_ENDOFNAMES(source, channel)                 ": 366 " + source + " " + channel + " :End of /NAMES list."
#define RPL_JOIN(nick, channel)                         ":" + nick + " JOIN " + channel
#define RPL_MODE(source, channel, modes, args)          ":" + source + " MODE " + channel + " " + modes + " " + args

class Channel
{
	private:
		std::string _name;
		std::string _topic;
		std::map<char, bool> _flags;
		std::vector<Client> _clients;
		std::vector<Client> _operator;

	public:
		Channel();
		Channel(const std::string& name);

		//get
		std::string			getName();
		std::vector<Client>	getClients();
		std::vector<Client>	getOperator();
		std::string 		getUsersNames();
		Client&				getClient(std::string target);
		bool 				IsFlags(char c);
		bool				IsOperator(Client &client);

		//set
		void ClientAdd(Client &newClient);
		void ClientRemove(Client &removeClient);
		void OperatorRemove(Client &removeClient);
		void OperatorAdd(Client &newOperator);
		void Brodcast(std::string &message);
};

#endif
