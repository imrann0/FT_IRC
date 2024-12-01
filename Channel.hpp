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

	public:
		Channel();
		Channel(const std::string& name);
};

#endif
