#include "Channel.hpp"
#include <algorithm>
#include <stdexcept>	// runtime_error
#include "Tools.hpp"

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
    it user = find(_clients.begin(), _clients.end(), removeClient);
    if (user != _clients.end())
    {
        _clients.erase(user);
        return ;
    }
    throw std::runtime_error("Channel Remove Error: Client Not Found");
    
}

void Channel::OperatorRemove(Client &removeClient)
{
    it user = find(_operator.begin(), _operator.end(), removeClient);
    if (user != _operator.end())
    {
        _operator.erase(user);
        return ;
    }
    throw std::runtime_error("Channel Remove Error: Client Not Found");
    
}


std::string Channel::getUsersNames()
{
    std::string usersNames = "";
    for (it user = _clients.begin(); user != _clients.end(); user++) {
        usersNames += user->getNickname() + " ";
    }
    if (!usersNames.empty())
        usersNames.erase(usersNames.size() - 1);
    return usersNames;
}

bool Channel::IsOperator(Client &client)
{
    it operatorClient = find(_operator.begin(), _operator.end(), client);
    if (operatorClient != _operator.end())
        return (true);
    return (false);
}

Client&  Channel::getClient(std::string target)
{
    for (it begin = _clients.begin(); begin != _clients.end(); begin++)
    {
        if (begin->getNickname() == target)
            return (*begin);
    }
    throw std::runtime_error("Channel Error: Client Not Found");
}

void Channel::Brodcast(std::string &message)
{
    for (it begin = _clients.begin(); begin != _clients.end(); begin++)
    {
        yolla(begin->getClientFd(), message);
    }
}