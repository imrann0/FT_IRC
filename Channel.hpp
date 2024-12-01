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
		std::vector<Client> _clients;
		std::vector<Client> _operator;

	public:
		Channel();
		Channel(const std::string& name);

		//get
		std::string			getName();
		std::vector<Client>	getClients();
		std::vector<Client>	getOperator();

		//set
		void ClientAdd(Client newClient);
		void OperatorAdd(Client newOperator);


};

#endif
