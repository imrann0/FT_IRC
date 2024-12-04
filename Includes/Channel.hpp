#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>
#include "Client.hpp"

typedef std::vector<Client>::iterator it;

class Channel
{
	private:
		std::string _name;
		std::string _topic;
		std::vector<Client> _clients;
		std::vector<Client> _operator;

	public:
		Channel();
		Channel(const std::string& name);

		//get
		std::string			getName();
		std::vector<Client>	getClients();
		std::vector<Client>	getOperator();
		//bool ExitOperatorStatus(Client &client);


		//set
		void ClientAdd(Client &newClient);
		void OperatorAdd(Client &newOperator);
		void ClientRemove(Client &removeClient);
};

#endif
