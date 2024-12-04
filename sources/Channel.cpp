#include "Channel.hpp"
#include <algorithm>

Channel::Channel() {}
Channel::Channel(const std::string& name) {_name = name;}
std::string Channel::getName() {return _name;}
std::vector<Client> Channel::getClients() {return _clients;}
std::vector<Client> Channel::getOperator() { return _operator; }

void Channel::ClientAdd(Client &newClient) { _clients.push_back(newClient);}
void Channel::OperatorAdd(Client &newOperator) { _operator.push_back(newOperator);}

void Channel::ClientRemove(Client &removeClient)
{
    it a = find(_clients.begin(), _clients.end(), removeClient);
    if (a != _clients.end())
    {
        _clients.erase(a);
    }
}
