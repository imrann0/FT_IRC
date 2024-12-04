#include "Channel.hpp"
#include <algorithm>
#include <stdexcept>	// runtime_error

Channel::Channel() {}
Channel::Channel(const std::string& name) 
{
    _name = name;
    _flags['i'] = false;
    _flags['t'] = false;
    _flags['k'] = false;
    _flags['l'] = false;
}
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
        return ;
    }
    throw std::runtime_error("Remove: Client Not Found");
    
}

std::string Channel::getUsersNames()
{
    std::string usersNames = "";

    for (it user = _clients.begin(); user != _clients.end(); user++)
    {
        usersNames.append(user->getNickname() + " ");
    }
    return (usersNames);
}