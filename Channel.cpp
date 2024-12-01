#include "Channel.hpp"

Channel::Channel() {}
Channel::Channel(const std::string& name) {_name = name;}
std::string Channel::getName() {return _name;}
std::vector<Client> Channel::getClients() {return _clients;}
std::vector<Client> Channel::getOperator() {return _operator;}

void Channel::ClientAdd(Client newClient) { _clients.push_back(newClient);}
void Channel::OperatorAdd(Client newOperator) { _operator.push_back(newOperator);}
